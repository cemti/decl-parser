namespace DeclParser
{
    using static BaseType;
    
    public partial class Wizard : Form
    {
        public string? Buffer { get; private set; }

        private readonly DataModel dataModel;

        public Wizard(DataModel dataModel)
        {
            InitializeComponent();
            this.dataModel = dataModel;
        }

        protected override void OnLoad(EventArgs e)
        {
            WizardAdder adder = new("Declaration") { Text = "Variable Declaration", Placeholder = "sample" };
            adder.ShowDialog();

            if (adder.Result is NamedDeclaration decl)
            {
                if (string.IsNullOrWhiteSpace(decl.Name))
                    decl.Name = "sample";
               
                treeView1.Nodes.Add(decl.ToString()).Tag = decl;
            }
            else
                Close();
        }

        private void BeforeSelectNode(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node == null)
                return;

            foreach (ToolStripItem c in addNodeButton.DropDownItems)
                c.Enabled = true;

            addNodeButton.Enabled = true; 

            reviewToolStripMenuItem.Enabled = removeNodeButton.Enabled =
            variadicToolStripMenuItem.Enabled = declarationToolStripMenuItem.Enabled = false;

            if (e.Node.Tag is FundamentalType or string)
            {
                addNodeButton.Enabled = false;
                removeNodeButton.Enabled = true;
            }
            else if (e.Node.Tag is NamedDeclaration)
            {
                declarationToolStripMenuItem.Enabled = false;
                reviewToolStripMenuItem.Enabled = true;

                if (e.Node != treeView1.Nodes[0])
                    removeNodeButton.Enabled = true;

                if (e.Node.Nodes.Count > 0)
                    addNodeButton.Enabled = false;
            }
            else if (e.Node.Tag is NamedType nType)
            {
                if (!nType.Instantiable)
                    addNodeButton.Enabled = false;
                else
                {
                    foreach (ToolStripItem c in addNodeButton.DropDownItems)
                        c.Enabled = false;

                    declarationToolStripMenuItem.Enabled = true;
                }

                removeNodeButton.Enabled = true;
            }
            else if (e.Node.Tag is FunctionType fType)
            {
                foreach (ToolStripItem c in addNodeButton.DropDownItems)
                    c.Enabled = false;

                if (fType.Parameters.Count > 0 && fType.Parameters[^1].Decl.Type is EllipsisType)
                    addNodeButton.Enabled = false;
                else
                    variadicToolStripMenuItem.Enabled = true;  

                declarationToolStripMenuItem.Enabled = true;
                removeNodeButton.Enabled = true;
            }
            else if (e.Node.Tag is CompositeType cType)
            {
                addNodeButton.Enabled = cType.Decay == null;
                removeNodeButton.Enabled = true;
            }
            else if (e.Node.Tag is BaseType)
                removeNodeButton.Enabled = true;
        }

        private void AddTypeToNode(TreeNode node, object obj)
        {
            TreeNode? inNode = null;

            if (obj is BaseType type)
            {
                inNode = node.Nodes.Add(Summary.Verbose(type, new TypeParser(dataModel), true));

                if (obj is FunctionType)
                {
                    var retNode = inNode.Nodes.Add("Return type");
                    retNode.Name = "return";
                    retNode.NodeFont = new Font(treeView1.Font, FontStyle.Italic);
                }                   

                inNode.Tag = type;

                if (node.Tag is CompositeType cType)
                    cType.Decay = type;
                else if (node.Tag is NamedDeclaration decl)
                    decl.Decl.Type = type;
                else if (node.Name == "return")
                    ((FunctionType)node.Parent.Tag).Decay = type;
            }
            else if (obj is NamedDeclaration decl)
            {
                static string NewGuid() => $"__decl_{Guid.NewGuid().ToString().Replace("-", string.Empty)}";

                var anon = string.IsNullOrWhiteSpace(decl.Name);

                if (node.Tag is EnumType eType)
                {
                    var name = anon ? NewGuid() : decl.Name;
                    inNode = node.Nodes.Add(name);
                    inNode.Tag = name;
                    eType.Enumerators.Add(name);
                }
                else
                {
                    if (node.Tag is FunctionType fType)
                        fType.Parameters.Add(decl);
                    else if (node.Tag is StructType sType)
                    {
                        if (anon)
                            decl.Name = NewGuid();

                        sType.Members.Add(decl);
                    }

                    if (decl.Decl.Type is EllipsisType)
                        inNode = node.Nodes.Add("...");
                    else
                    {
                        inNode = node.Nodes.Add(decl.ToString());

                        if (anon)
                            inNode.NodeFont = new Font(treeView1.Font, FontStyle.Italic);

                        AddTypeToNode(inNode, decl.Decl.Type);
                    }

                    inNode.Tag = decl;
                }
            }

            if (inNode != null)
            {
                if (obj is CompositeType cType2 && cType2.Decay != null)
                    AddTypeToNode(inNode, cType2.Decay);
                else
                    treeView1.SelectedNode = inNode;
            }
        }

        private void RemoveNode(object sender, EventArgs e)
        {
            var node = treeView1.SelectedNode;
            var parent = node.Parent;

            if (parent.Tag is NamedDeclaration decl)
                decl.Decl.Type = null;
            else if (parent.Tag is CompositeType cType)
                cType.Decay = null;
            else if (parent.Tag is FunctionType fType)
                fType.Parameters.Remove((NamedDeclaration)node.Tag);
            else if (parent.Tag is StructType sType)
                sType.Members.Remove((NamedDeclaration)node.Tag);
            else if (parent.Tag is EnumType eType)
                eType.Enumerators.Remove((string)node.Tag);
            else if (parent.Name == "return")
                ((FunctionType)parent.Parent.Tag).Decay = null;

            node.Remove();
        }

        private void AddTypeToNode(object obj) => AddTypeToNode(treeView1.SelectedNode, obj);

        private void AddTypeOnClick(object sender, EventArgs e)
        {
            WizardAdder wizard = new(((ToolStripItem)sender).Text, treeView1.SelectedNode.Tag is EnumType);
            wizard.ShowDialog();
            
            if (wizard.Result is BaseType type)
                AddTypeToNode(type);
            else if (wizard.Result is NamedDeclaration decl)
                AddTypeToNode(decl);
        }

        private void AddFunctionOnClick(object sender, EventArgs e) => AddTypeToNode(new FunctionType(null, new()));
        private void AddVariadicOnClick(object sender, EventArgs e) => AddTypeToNode(new NamedDeclaration(null, new(new EllipsisType(), 0)));

        private void AddFundamentalTypeOnClick(object sender, EventArgs e)
        {
            WizardAdder wizard = new(((ToolStripItem)sender).Text, dataModel);
            wizard.ShowDialog();

            if (wizard.Result is FundamentalType fType)
                AddTypeToNode(fType);
        }

        private void Commit(object sender, EventArgs e)
        {
            try
            {
                Buffer = new Summary(new TypeParser(dataModel)
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
                var form = new Summary(new TypeParser(dataModel)
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
