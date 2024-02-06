using System.Data;
using System.Text;

namespace DeclParser
{
    using static BaseType;

    public partial class Summary : Form
    {
        private readonly TypeParser _parser;
        private readonly HashSet<string> _nameSet = new();
        private readonly StringBuilder _buffer = new();

        public string Buffer => _buffer.ToString();

        public Summary(TypeParser parser)
        {
            InitializeComponent();
            _parser = parser;

            PopulateText(parser.Variables, _buffer);
            textBox1.Text = Buffer;

            foreach (var (name, declaration) in parser.Variables)
            {
                if (declaration == null || declaration.Type == null)
                    throw new ArgumentException("Declaration is null.");
                
                AddNode(AddNode(treeView1.Nodes, name, new NamedDeclaration(name, declaration)), declaration.Type);
            }
        }

        public static string Verbose(BaseType? type, TypeParser parser, bool simple = false)
        {
            StringBuilder name = new();

            while (type != null)
            {
                foreach (var qualifiers in Enum.GetValues<Qualifiers>())
                    if (type.Qualifier.HasFlag(qualifiers))
                        name.Append(qualifiers == Qualifiers.@const ? "constant " : qualifiers.ToString() + " ");

                if (type is CompositeType cType)
                {
                    var deadEnd = cType.Decay == null;

                    if (type is PointerType pType)
                    {
                        name.Append("pointer");

                        if (simple || deadEnd)
                            break;

                        name.Append(" to a ");
                    }
                    else if (type is ArrayType aType)
                    {
                        name.Append("fixed array with ");

                        if (aType.Count.HasValue)
                        {
                            name.Append($"{aType.Count.Value} instance");

                            if (aType.Count.Value != 1)
                                name.Append('s');
                        }
                        else
                            name.Append("any instances");

                        if (simple || deadEnd)
                            break;

                        name.Append(" of a ");
                    }
                    else if (type is FunctionType fType1)
                    {
                        name.Append("function");

                        if (simple)
                            break;

                        if (fType1.HasNoParameters)
                            name.Append(" with no parameters");
                        else
                        {
                            name.Append($" with {fType1.Parameters.Count} parameter");

                            if (fType1.Parameters.Count != 1)
                                name.Append('s');

                            name.Append(" (");
                            name.AppendJoin(", ", fType1.Parameters.Select(a => TypeToString(a.Declaration.Type, a.Name, false)));
                            name.Append(')');
                        }

                        if (deadEnd)
                            break;

                        name.Append(", returning a ");
                    }

                    type = cType.Decay;
                }
                else
                {
                    if (type is FundamentalType fType)
                    {
                        if (fType.Sign != FundamentalType.TypeSign.None)
                            name.Append(fType.Sign + " ");

                        switch (fType.Type)
                        {
                            case FundamentalType.DataType.@void:
                                name.Append("void type");
                                break;

                            case FundamentalType.DataType.@char:
                                name.Append("character");
                                break;

                            case FundamentalType.DataType.@int:
                                name.Append("whole number");
                                break;

                            case FundamentalType.DataType.@float:
                            case FundamentalType.DataType.@double:
                                name.Append("floating point number");
                                break;
                        }

                        int size = parser.SizeOf(type);

                        if (size > 0)
                        {
                            name.Append($" that occupies {size} byte");

                            if (size != 1)
                                name.Append('s');
                        }
                    }
                    else if (type is NamedType nType)
                    {
                        if (!nType.Instantiable)
                            name.Append("incomplete ");

                        if (type is StructType sType)
                            name.Append(sType.IsUnion ? "union" : "structure");
                        else if (type is EnumType)
                            name.Append("enumeration");

                        name.Append($" {(nType.Anonymous ? "that is anonymous" : $"named {nType.Name}")}");
                    }
                    else if (type is EllipsisType)
                        name.Append("Ellipsis");

                    break;
                }
            }

            return name.ToString();
        }

        private TreeNode AddNode(TreeNodeCollection parent, object? rawName, object tag)
        {
            var name = rawName?.ToString();
            var empty = string.IsNullOrWhiteSpace(name);
            
            var node = parent.Add(empty ? "Unnamed" : name);
            node.Tag = tag;

            if (empty)
                node.NodeFont = new Font(treeView1.Font, FontStyle.Italic);

            return node;
        }

        private TreeNode AddNode(TreeNode node, object? name, object tag) => AddNode(node.Nodes, name, tag);

        private void AddNode(TreeNode node, object tag) => AddNode(node.Nodes, tag, tag);

        private void PopulateText(NamedType nType, StringBuilder sb, bool allowTypedef = false)
        {
            var anon = nType.Anonymous && allowTypedef;

            if (anon)
                sb.Append("typedef ");

            sb.Append(TypeToString(nType, null, anon));

            if (nType.Instantiable)
            {
                _nameSet.Add(nType.Name);
                sb.AppendLine(" {");

                if (nType is StructType sType)
                    PopulateText(sType.Members, sb, true);
                else if (nType is EnumType eType)
                    sb.AppendLine("  " + new StringBuilder().AppendJoin(", ", eType.Enumerators));

                sb.Append('}');

                if (anon)
                    sb.Append($" {nType.Name}");

                sb.AppendLine(";");
                sb.AppendLine();
            }
            else
                sb.AppendLine(";");
        }

        private void PopulateText(IList<NamedDeclaration> list, StringBuilder sb, bool depth = false)
        {
            void Recursive(BaseType tType)
            {
                while (tType is CompositeType cType)
                {
                    if (tType is FunctionType fType)
                        foreach (var (_, d) in fType.Parameters)
                            Recursive(d.Type);

                    tType = cType.Decay;
                }

                if (tType is NamedType nType && nType.Instantiable && !_nameSet.Contains(nType.Name))
                {
                    if (nType.Qualifier != Qualifiers.None)
                    {
                        nType = (NamedType)nType.Clone();
                        nType.Qualifier = default;
                    }

                    StringBuilder sb2 = new();
                    PopulateText(nType, sb2, true);
                    sb.Insert(0, sb2);
                }
            }
            
            foreach (var v in list)
            {
                Recursive(v.Declaration.Type);

                if (depth)
                    sb.Append("  ");

                sb.AppendLine($"{v};");
            }
        }

        private void BeforeNodeSelection(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node == null)
                return;

            StringBuilder sb = new();

            if (e.Node.Tag is NamedDeclaration declaration)
                PopulateText(new List<NamedDeclaration> { declaration }, sb);
            else if (e.Node.Tag is BaseType type)
            {
                sb.AppendLine($"// {Verbose(type, _parser)}");

                if (type is NamedType nType)
                    PopulateText(nType, sb, true);
                else
                    PopulateText(new List<NamedDeclaration> { new(null, new Declaration(type, 0)) }, sb);
            }
            else if (e.Node.Tag is List<NamedDeclaration> declarations)
                PopulateText(declarations, sb);
            else
                sb.Append(e.Node.Tag);

            textBox2.Text = sb.ToString();
        }

        private void BeforeNodeExpansion(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node == null)
                return;

            foreach (TreeNode node in e.Node.Nodes)
            {
                if (node.Name != string.Empty)
                    continue;

                node.Name = "visited";

                if (node.Tag is NamedDeclaration d)
                    AddNode(node, d.Declaration.Type);
                else if (node.Tag is BaseType type)
                {
                    if (type is CompositeType cType)
                    {
                        AddNode(node, cType.Decay);

                        if (type is FunctionType fType && !fType.HasNoParameters)
                        {
                            var pNode = node.Nodes.Add($"Parameters ({fType.Parameters.Count})");
                            pNode.Tag = fType.Parameters;
                            pNode.NodeFont = new Font(treeView1.Font, FontStyle.Bold);

                            foreach (var parameter in fType.Parameters)
                            {
                                if (parameter.Declaration.Type is EllipsisType)
                                    AddNode(pNode, "...");
                                else
                                    AddNode(pNode, parameter.Name, parameter);
                            }
                        }
                    }
                    else if (type is StructType sType)
                    {
                        if (sType.Instantiable && sType.Members.Count > 0)
                        {
                            var sNode = node.Nodes.Add($"Members ({sType.Members.Count})");
                            sNode.Tag = sType.Members;
                            sNode.NodeFont = new Font(treeView1.Font, FontStyle.Bold);

                            foreach (var memb in sType.Members)
                                AddNode(sNode, memb.Name, memb);
                        }
                    }
                    else if (type is EnumType eType)
                        foreach (var name in eType.Enumerators)
                            AddNode(node, name);

                    int size = _parser.SizeOf(type);

                    if (size > 0)
                    {
                        var sizeInfo = $"Size: {size} byte";

                        if (size != 1)
                            sizeInfo += 's';

                        AddNode(node, sizeInfo, size).NodeFont = new Font(treeView1.Font, FontStyle.Bold);
                    }
                }
            }
        }
    }
}
