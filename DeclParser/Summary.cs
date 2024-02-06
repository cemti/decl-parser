using System.Data;
using System.Text;

namespace DeclParser
{
    using static BaseType;

    public partial class Summary : Form
    {
        private readonly TypeParser parser;
        private readonly HashSet<string> readNT = new();
        private readonly StringBuilder buffer = new();

        public string Buffer => buffer.ToString();

        public Summary(TypeParser parser)
        {
            InitializeComponent();
            this.parser = parser;

            PopulateText(parser.Variables, buffer);
            textBox1.Text = Buffer;

            foreach (var (name, decl) in parser.Variables)
            {
                if (decl == null || decl.Type == null)
                    throw new ArgumentException("Declaration is null.");
                
                AddNode(AddNode(treeView1.Nodes, name, new NamedDeclaration(name, decl)), decl.Type);
            }
        }

        public static string Verbose(BaseType? type, TypeParser parser, bool simple = false)
        {
            StringBuilder str = new();

            while (type != null)
            {
                foreach (var q in Enum.GetValues<Qualifiers>())
                    if (type.Qualifier.HasFlag(q))
                        str.Append(q == Qualifiers.@const ? "constant " : q.ToString() + " ");

                if (type is CompositeType cType)
                {
                    var deadEnd = cType.Decay == null;

                    if (type is PointerType pType)
                    {
                        str.Append("pointer");

                        if (simple || deadEnd)
                            break;

                        str.Append(" to a ");
                    }
                    else if (type is ArrayType aType)
                    {
                        str.Append("fixed array with ");

                        if (aType.Count.HasValue)
                        {
                            str.Append($"{aType.Count.Value} instance");

                            if (aType.Count.Value != 1)
                                str.Append('s');
                        }
                        else
                            str.Append("any instances");

                        if (simple || deadEnd)
                            break;

                        str.Append(" of a ");
                    }
                    else if (type is FunctionType fType1)
                    {
                        str.Append("function");

                        if (simple)
                            break;

                        if (fType1.HasNoParams)
                            str.Append(" with no parameters");
                        else
                        {
                            str.Append($" with {fType1.Parameters.Count} parameter");

                            if (fType1.Parameters.Count != 1)
                                str.Append('s');

                            str.Append(" (");
                            str.AppendJoin(", ", fType1.Parameters.Select(a => TypeToString(a.Decl.Type, a.Name, false)));
                            str.Append(')');
                        }

                        if (deadEnd)
                            break;

                        str.Append(", returning a ");
                    }

                    type = cType.Decay;
                }
                else
                {
                    if (type is FundamentalType fType)
                    {
                        if (fType.HasSign)
                            str.Append(fType.Sign + " ");

                        switch (fType.Type)
                        {
                            case FundamentalType.Types.@void:
                                str.Append("void type");
                                break;

                            case FundamentalType.Types.@char:
                                str.Append("character");
                                break;

                            case FundamentalType.Types.@int:
                                str.Append("whole number");
                                break;

                            case FundamentalType.Types.@float:
                            case FundamentalType.Types.@double:
                                str.Append("floating point number");
                                break;
                        }

                        int sz = parser.SizeOf(type);

                        if (sz > 0)
                        {
                            str.Append($" that occupies {sz} byte");

                            if (sz != 1)
                                str.Append('s');
                        }
                    }
                    else if (type is NamedType nType)
                    {
                        if (!nType.Instantiable)
                            str.Append("incomplete ");

                        if (type is StructType sType)
                            str.Append(sType.IsUnion ? "union" : "structure");
                        else if (type is EnumType)
                            str.Append("enumeration");

                        str.Append($" {(nType.Anonymous ? "that is anonymous" : $"named {nType.Name}")}");
                    }
                    else if (type is EllipsisType)
                        str.Append("Ellipsis");

                    break;
                }
            }

            return str.ToString();
        }

        private TreeNode AddNode(TreeNodeCollection parent, object? name, object tag)
        {
            var str = name?.ToString();
            var empty = string.IsNullOrWhiteSpace(str);
            
            var node = parent.Add(empty ? "Unnamed" : str);
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
                readNT.Add(nType.Name);
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

        private void PopulateText(List<NamedDeclaration> list, StringBuilder sb, bool depth = false)
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

                if (tType is NamedType nType && nType.Instantiable && !readNT.Contains(nType.Name))
                {
                    if (nType.HasQualifier)
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
                Recursive(v.Decl.Type);

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

            if (e.Node.Tag is NamedDeclaration decl)
                PopulateText(new List<NamedDeclaration> { decl }, sb);
            else if (e.Node.Tag is BaseType type)
            {
                sb.AppendLine($"// {Verbose(type, parser)}");

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
                    AddNode(node, d.Decl.Type);
                else if (node.Tag is BaseType type)
                {
                    if (type is CompositeType cType)
                    {
                        AddNode(node, cType.Decay);

                        if (type is FunctionType fType && !fType.HasNoParams)
                        {
                            var pNode = node.Nodes.Add($"Parameters ({fType.Parameters.Count})");
                            pNode.Tag = fType.Parameters;
                            pNode.NodeFont = new Font(treeView1.Font, FontStyle.Bold);

                            foreach (var param in fType.Parameters)
                            {
                                if (param.Decl.Type is EllipsisType)
                                    AddNode(pNode, "...");
                                else
                                    AddNode(pNode, param.Name, param);
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

                    int sz = parser.SizeOf(type);

                    if (sz > 0)
                    {
                        var str = $"Size: {sz} byte";

                        if (sz != 1)
                            str += 's';

                        AddNode(node, str, sz).NodeFont = new Font(treeView1.Font, FontStyle.Bold);
                    }
                }
            }
        }
    }
}
