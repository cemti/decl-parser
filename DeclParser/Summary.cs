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
                if (declaration?.Type is null)
                    throw new ArgumentException("Declaration is null.");
                
                AddNode(AddNode(treeView1.Nodes, name, new NamedDeclaration(name, declaration)), declaration.Type);
            }
        }

        public static string Verbose(BaseType? type, TypeParser parser, bool simple = false)
        {
            StringBuilder name = new();

            while (type is not null)
            {
                {
                    var qualifiers = QualifiersToString(type.Qualifiers);

                    if (!string.IsNullOrEmpty(qualifiers))
                        name.Append(qualifiers.ToLower().Replace("const", "constant")).Append(' ');
                }

                if (type is CompositeType cType)
                {
                    var deadEnd = cType.Decay is null;

                    switch (type)
                    {
                        case PointerType pType:
                            name.Append("pointer");

                            if (simple || deadEnd)
                                goto outer;

                            name.Append(" to a ");
                            break;

                        case ArrayType aType:
                            name.Append("fixed array with ");

                            if (aType.Count.HasValue)
                            {
                                var count = (int)aType.Count;
                                name.Append(count).Append(" instance");

                                if (count != 1)
                                    name.Append('s');
                            }
                            else
                                name.Append("any instances");

                            if (simple || deadEnd)
                                goto outer;

                            name.Append(" of a ");
                            break;

                        case FunctionType fType1:
                            name.Append("function");

                            if (simple)
                                goto outer;

                            if (!fType1.HasParameters)
                                name.Append(" with no parameters");
                            else
                            {
                                name.Append($" with {fType1.Parameters.Count} parameter");

                                if (fType1.Parameters.Count != 1)
                                    name.Append('s');

                                name.Append(" (")
                                    .AppendJoin(", ", fType1.Parameters.Select(a => TypeToString(a.Declaration.Type, a.Name, false)))
                                    .Append(')');
                            }

                            if (deadEnd)
                                goto outer;

                            name.Append(", returning a ");
                            break;
                    }

                    type = cType.Decay;
                }
                else
                {
                    switch (type)
                    {
                        case FundamentalType fType:
                            if (fType.Sign != FundamentalType.TypeSign.None)
                                name.Append(fType.Sign).Append(' ');

                            name.Append(fType.Type switch
                            {
                                FundamentalType.DataType.Void => "void type",
                                FundamentalType.DataType.Char => "character",
                                FundamentalType.DataType.Int => "whole number",
                                FundamentalType.DataType.Float or FundamentalType.DataType.Double => "floating point number",
                                _ => "fundamental type"
                            });

                            int size = parser.SizeOf(type);

                            if (size > 0)
                            {
                                name.Append($" that occupies {size} byte");

                                if (size != 1)
                                    name.Append('s');
                            }

                            break;

                        case NamedType nType:
                            if (!nType.Instantiable)
                                name.Append("incomplete ");

                            name
                                .Append(type switch
                                {
                                    StructType sType => sType.IsUnion ? "union" : "structure",
                                    EnumType => "enumeration",
                                    _ => "custom type"
                                })
                                .Append($" {(nType.Anonymous ? "that is anonymous" : $"named {nType.Name}")}");

                            break;

                        case EllipsisType:
                            name.Append("ellipsis");
                            break;
                    }

                    goto outer;
                }
            }

        outer:
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

                switch (nType)
                {
                    case StructType sType:
                        PopulateText(sType.Members, sb, true);
                        break;

                    case EnumType eType:
                        sb.Append("  ").AppendLine(string.Join(", ", eType.Enumerators));
                        break;
                }

                sb.Append('}');

                if (anon)
                    sb.Append(' ').Append(nType.Name);

                sb.AppendLine(";").AppendLine();
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
                        foreach (var (_, decl) in fType.Parameters)
                            Recursive(decl.Type);

                    tType = cType.Decay;
                }

                if (tType is NamedType nType && nType.Instantiable && !_nameSet.Contains(nType.Name))
                {
                    if (nType.Qualifiers != TypeQualifiers.None)
                    {
                        nType = (NamedType)nType.Clone();
                        nType.Qualifiers = TypeQualifiers.None;
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
            if (e.Node is null)
                return;

            StringBuilder sb = new();

            switch (e.Node.Tag)
            {
                case NamedDeclaration declaration:
                    PopulateText(new List<NamedDeclaration> { declaration }, sb);
                    break;

                case BaseType type:
                    sb.AppendLine($"// {Verbose(type, _parser)}");

                    if (type is NamedType nType)
                        PopulateText(nType, sb, true);
                    else
                        PopulateText(new List<NamedDeclaration> { new(null, new Declaration(type, 0)) }, sb);

                    break;

                case List<NamedDeclaration> declarations:
                    PopulateText(declarations, sb);
                    break;

                default:
                    sb.Append(e.Node.Tag);
                    break;
            }

            textBox2.Text = sb.ToString();
        }

        private void BeforeNodeExpansion(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node is null)
                return;

            foreach (TreeNode node in e.Node.Nodes)
            {
                if (node.Name != string.Empty)
                    continue;

                node.Name = "visited";

                switch (node.Tag)
                {
                    case NamedDeclaration declaration:
                        AddNode(node, declaration.Declaration.Type);
                        break;

                    case BaseType type:
                        switch (type)
                        {
                            case CompositeType cType:
                                AddNode(node, cType.Decay);

                                if (type is FunctionType { HasParameters: true } fType)
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

                                break;

                            case StructType sType:
                                if (sType is { Instantiable: true, Members.Count: > 0 })
                                {
                                    var sNode = node.Nodes.Add($"Members ({sType.Members.Count})");
                                    sNode.Tag = sType.Members;
                                    sNode.NodeFont = new Font(treeView1.Font, FontStyle.Bold);

                                    foreach (var memb in sType.Members)
                                        AddNode(sNode, memb.Name, memb);
                                }

                                break;

                            case EnumType enumType:
                                foreach (var name in enumType.Enumerators)
                                    AddNode(node, name);

                                break;
                        }

                        int size = _parser.SizeOf(type);

                        if (size > 0)
                        {
                            var sizeInfo = $"Size: {size} byte";

                            if (size != 1)
                                sizeInfo += 's';

                            AddNode(node, sizeInfo, size).NodeFont = new Font(treeView1.Font, FontStyle.Bold);
                        }

                        break;
                }
            }
        }
    }
}
