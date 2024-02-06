namespace DeclParser
{
    using static BaseType;
    
    public partial class Wizard : Form
    {
        private readonly DataModel _dataModel;

        public string? Buffer { get; private set; }

        public Wizard(DataModel dataModel)
        {
            InitializeComponent();
            _dataModel = dataModel;
        }

        protected override void OnLoad(EventArgs e)
        {
            using WizardAdder adder = new("Declaration") { Text = "Variable Declaration", Placeholder = "sample" };
            adder.ShowDialog();

            if (adder.Result is NamedDeclaration declaration)
            {
                if (string.IsNullOrWhiteSpace(declaration.Name))
                    declaration.Name = "sample";
               
                treeView1.Nodes.Add(declaration.ToString()).Tag = declaration;
            }
            else
                Close();
        }

        private void BeforeSelectNode(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node is null)
                return;

            foreach (ToolStripItem item in addNodeButton.DropDownItems)
                item.Enabled = true;

            addNodeButton.Enabled = true; 

            reviewToolStripMenuItem.Enabled = removeNodeButton.Enabled =
            variadicToolStripMenuItem.Enabled = declarationToolStripMenuItem.Enabled = false;

            switch (e.Node.Tag)
            {
                case FundamentalType or string:
                    addNodeButton.Enabled = false;
                    removeNodeButton.Enabled = true;
                    break;

                case NamedDeclaration:
                    declarationToolStripMenuItem.Enabled = false;
                    reviewToolStripMenuItem.Enabled = true;

                    if (e.Node != treeView1.Nodes[0])
                        removeNodeButton.Enabled = true;

                    if (e.Node.Nodes.Count > 0)
                        addNodeButton.Enabled = false;

                    break;

                case NamedType nType:
                    if (nType.Instantiable)
                    {
                        foreach (ToolStripItem c in addNodeButton.DropDownItems)
                            c.Enabled = false;

                        declarationToolStripMenuItem.Enabled = true;
                    }
                    else
                        addNodeButton.Enabled = false;

                    removeNodeButton.Enabled = true;
                    break;

                case FunctionType fType:
                    foreach (ToolStripItem c in addNodeButton.DropDownItems)
                        c.Enabled = false;

                    if (fType.Parameters.Count > 0 && fType.Parameters[^1].Declaration.Type is EllipsisType)
                        addNodeButton.Enabled = false;
                    else
                        variadicToolStripMenuItem.Enabled = true;

                    declarationToolStripMenuItem.Enabled = true;
                    removeNodeButton.Enabled = true;
                    break;

                case CompositeType cType:
                    addNodeButton.Enabled = cType.Decay is null;
                    removeNodeButton.Enabled = true;
                    break;

                case BaseType:
                    removeNodeButton.Enabled = true;
                    break;
            }
        }

        private void AddTypeToNode(TreeNode node, object obj)
        {
            TreeNode? inNode = null;

            switch (obj)
            {
                case BaseType type:
                    inNode = node.Nodes.Add(Summary.Verbose(type, new TypeParser(_dataModel), true));

                    if (obj is FunctionType)
                    {
                        var retNode = inNode.Nodes.Add("Return type");
                        retNode.Name = "return";
                        retNode.NodeFont = new Font(treeView1.Font, FontStyle.Italic);
                    }

                    inNode.Tag = type;

                    switch (node.Tag)
                    {
                        case CompositeType cType:
                            cType.Decay = type;
                            break;

                        case NamedDeclaration declaration:
                            declaration.Declaration.Type = type;
                            break;

                        case var _ when node.Name == "return":
                            ((FunctionType)node.Parent.Tag).Decay = type;
                            break;
                    }

                    break;

                case NamedDeclaration declaration:
                    static string NewUniqueName() => $"__decl_{Guid.NewGuid().ToString().Replace("-", string.Empty)}";

                    var anon = string.IsNullOrWhiteSpace(declaration.Name);

                    if (node.Tag is EnumType eType)
                    {
                        var name = anon ? NewUniqueName() : declaration.Name;
                        inNode = node.Nodes.Add(name);
                        inNode.Tag = name;
                        eType.Enumerators.Add(name);
                    }
                    else
                    {
                        if (node.Tag is FunctionType fType)
                            fType.Parameters.Add(declaration);
                        else if (node.Tag is StructType sType)
                        {
                            if (anon)
                                declaration.Name = NewUniqueName();

                            sType.Members.Add(declaration);
                        }

                        if (declaration.Declaration.Type is EllipsisType)
                            inNode = node.Nodes.Add("...");
                        else
                        {
                            inNode = node.Nodes.Add(declaration.ToString());

                            if (anon)
                                inNode.NodeFont = new Font(treeView1.Font, FontStyle.Italic);

                            AddTypeToNode(inNode, declaration.Declaration.Type);
                        }

                        inNode.Tag = declaration;
                    }

                    break;
            }

            if (inNode is not null)
            {
                if (obj is CompositeType { Decay: not null } cType2)
                    AddTypeToNode(inNode, cType2.Decay);
                else
                    treeView1.SelectedNode = inNode;
            }
        }

        private void RemoveNode(object sender, EventArgs e)
        {
            var node = treeView1.SelectedNode;
            var parent = node.Parent;

            switch (parent.Tag)
            {
                case NamedDeclaration declaration:
                    declaration.Declaration.Type = null;
                    break;

                case FunctionType fType:
                    fType.Parameters.Remove((NamedDeclaration)node.Tag);
                    break;

                case CompositeType cType:
                    cType.Decay = null;
                    break;

                case StructType sType:
                    sType.Members.Remove((NamedDeclaration)node.Tag);
                    break;

                case EnumType eType:
                    eType.Enumerators.Remove((string)node.Tag);
                    break;

                case var _ when parent.Name == "return":
                    ((FunctionType)parent.Parent.Tag).Decay = null;
                    break;
            }

            node.Remove();
        }

        private void AddTypeToNode(object obj) => AddTypeToNode(treeView1.SelectedNode, obj);

        private void AddTypeOnClick(object sender, EventArgs e)
        {
            using WizardAdder wizard = new(((ToolStripItem)sender).Text, treeView1.SelectedNode.Tag is EnumType);
            wizard.ShowDialog();

            if (wizard.Result is BaseType or NamedDeclaration)
                AddTypeToNode(wizard.Result);
        }

        private void AddFunctionOnClick(object sender, EventArgs e) => AddTypeToNode(new FunctionType(null, new List<NamedDeclaration>()));
        private void AddVariadicOnClick(object sender, EventArgs e) => AddTypeToNode(new NamedDeclaration(null, new(new EllipsisType(), 0)));

        private void AddFundamentalTypeOnClick(object sender, EventArgs e)
        {
            using WizardAdder wizard = new(((ToolStripItem)sender).Text, _dataModel);
            wizard.ShowDialog();

            if (wizard.Result is FundamentalType fType)
                AddTypeToNode(fType);
        }

        private void Commit(object sender, EventArgs e)
        {
            try
            {
                Buffer = new Summary(new TypeParser(_dataModel)
                {
                    DeclArray = new[] { new List<NamedDeclaration>() { (NamedDeclaration)treeView1.Nodes[0].Tag } }
                }).Buffer;

                toolStripLabel1.Text = string.Empty;
                DialogResult = DialogResult.OK;
                Close();
            }
            catch (Exception ex)
            {
                toolStripLabel1.Text = ex.Message;
            }
        }

        private void Review(object sender, EventArgs e)
        {
            try
            {
                using Summary form = new(new(_dataModel)
                {
                    DeclArray = new[] { new List<NamedDeclaration>() { (NamedDeclaration)treeView1.SelectedNode.Tag } }
                });

                toolStripLabel1.Text = string.Empty;
                form.ShowDialog();
            }
            catch (Exception ex)
            {
                toolStripLabel1.Text = ex.Message;
            }
        }
    }
}
