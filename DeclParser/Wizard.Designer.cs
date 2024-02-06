namespace DeclParser
{
    partial class Wizard
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Wizard));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.declarationButton = new System.Windows.Forms.ToolStripDropDownButton();
            this.commitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.reviewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addNodeButton = new System.Windows.Forms.ToolStripDropDownButton();
            this.declarationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.primitiveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.characterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.charToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.signedCharToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unsignedCharToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.integerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.signedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.intToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.shortIntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.longIntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.longLongIntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unsignedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unsignedIntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unsignedLongIntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unsignedShortIntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unsignedLongLongIntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.floatingPointNumberToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.floatToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.doubleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.longDoubleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.voidToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pointerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.arrayToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.functionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.customToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.variadicToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeNodeButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.declarationButton,
            this.addNodeButton,
            this.removeNodeButton,
            this.toolStripLabel1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Padding = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.toolStrip1.Size = new System.Drawing.Size(800, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // declarationButton
            // 
            this.declarationButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.declarationButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.commitToolStripMenuItem,
            this.reviewToolStripMenuItem});
            this.declarationButton.Image = ((System.Drawing.Image)(resources.GetObject("declarationButton.Image")));
            this.declarationButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.declarationButton.Name = "declarationButton";
            this.declarationButton.Size = new System.Drawing.Size(80, 22);
            this.declarationButton.Text = "Declaration";
            // 
            // commitToolStripMenuItem
            // 
            this.commitToolStripMenuItem.Name = "commitToolStripMenuItem";
            this.commitToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.commitToolStripMenuItem.Text = "Commit";
            this.commitToolStripMenuItem.Click += new System.EventHandler(this.Commit);
            // 
            // reviewToolStripMenuItem
            // 
            this.reviewToolStripMenuItem.Name = "reviewToolStripMenuItem";
            this.reviewToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.reviewToolStripMenuItem.Text = "Review selection";
            this.reviewToolStripMenuItem.Click += new System.EventHandler(this.Review);
            // 
            // addNodeButton
            // 
            this.addNodeButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.addNodeButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.declarationToolStripMenuItem,
            this.primitiveToolStripMenuItem,
            this.pointerToolStripMenuItem,
            this.arrayToolStripMenuItem,
            this.functionToolStripMenuItem,
            this.customToolStripMenuItem,
            this.variadicToolStripMenuItem});
            this.addNodeButton.Enabled = false;
            this.addNodeButton.Image = ((System.Drawing.Image)(resources.GetObject("addNodeButton.Image")));
            this.addNodeButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addNodeButton.Name = "addNodeButton";
            this.addNodeButton.Size = new System.Drawing.Size(72, 22);
            this.addNodeButton.Text = "Add node";
            // 
            // declarationToolStripMenuItem
            // 
            this.declarationToolStripMenuItem.Name = "declarationToolStripMenuItem";
            this.declarationToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.declarationToolStripMenuItem.Text = "Declaration";
            this.declarationToolStripMenuItem.Click += new System.EventHandler(this.AddTypeOnClick);
            // 
            // primitiveToolStripMenuItem
            // 
            this.primitiveToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.characterToolStripMenuItem,
            this.integerToolStripMenuItem,
            this.floatingPointNumberToolStripMenuItem,
            this.voidToolStripMenuItem});
            this.primitiveToolStripMenuItem.Name = "primitiveToolStripMenuItem";
            this.primitiveToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.primitiveToolStripMenuItem.Text = "Primitive";
            // 
            // characterToolStripMenuItem
            // 
            this.characterToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.charToolStripMenuItem,
            this.signedCharToolStripMenuItem,
            this.unsignedCharToolStripMenuItem});
            this.characterToolStripMenuItem.Name = "characterToolStripMenuItem";
            this.characterToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.characterToolStripMenuItem.Text = "Character";
            // 
            // charToolStripMenuItem
            // 
            this.charToolStripMenuItem.Name = "charToolStripMenuItem";
            this.charToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.charToolStripMenuItem.Text = "char";
            this.charToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // signedCharToolStripMenuItem
            // 
            this.signedCharToolStripMenuItem.Name = "signedCharToolStripMenuItem";
            this.signedCharToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.signedCharToolStripMenuItem.Text = "signed char";
            this.signedCharToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // unsignedCharToolStripMenuItem
            // 
            this.unsignedCharToolStripMenuItem.Name = "unsignedCharToolStripMenuItem";
            this.unsignedCharToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.unsignedCharToolStripMenuItem.Text = "unsigned char";
            this.unsignedCharToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // integerToolStripMenuItem
            // 
            this.integerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.signedToolStripMenuItem,
            this.unsignedToolStripMenuItem});
            this.integerToolStripMenuItem.Name = "integerToolStripMenuItem";
            this.integerToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.integerToolStripMenuItem.Text = "Integer";
            // 
            // signedToolStripMenuItem
            // 
            this.signedToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.intToolStripMenuItem,
            this.shortIntToolStripMenuItem,
            this.longIntToolStripMenuItem,
            this.longLongIntToolStripMenuItem});
            this.signedToolStripMenuItem.Name = "signedToolStripMenuItem";
            this.signedToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.signedToolStripMenuItem.Text = "signed";
            // 
            // intToolStripMenuItem
            // 
            this.intToolStripMenuItem.Name = "intToolStripMenuItem";
            this.intToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.intToolStripMenuItem.Text = "int";
            this.intToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // shortIntToolStripMenuItem
            // 
            this.shortIntToolStripMenuItem.Name = "shortIntToolStripMenuItem";
            this.shortIntToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.shortIntToolStripMenuItem.Text = "short int";
            this.shortIntToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // longIntToolStripMenuItem
            // 
            this.longIntToolStripMenuItem.Name = "longIntToolStripMenuItem";
            this.longIntToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.longIntToolStripMenuItem.Text = "long int";
            this.longIntToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // longLongIntToolStripMenuItem
            // 
            this.longLongIntToolStripMenuItem.Name = "longLongIntToolStripMenuItem";
            this.longLongIntToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.longLongIntToolStripMenuItem.Text = "long long int";
            this.longLongIntToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // unsignedToolStripMenuItem
            // 
            this.unsignedToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.unsignedIntToolStripMenuItem,
            this.unsignedLongIntToolStripMenuItem,
            this.unsignedShortIntToolStripMenuItem,
            this.unsignedLongLongIntToolStripMenuItem});
            this.unsignedToolStripMenuItem.Name = "unsignedToolStripMenuItem";
            this.unsignedToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.unsignedToolStripMenuItem.Text = "unsigned";
            // 
            // unsignedIntToolStripMenuItem
            // 
            this.unsignedIntToolStripMenuItem.Name = "unsignedIntToolStripMenuItem";
            this.unsignedIntToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
            this.unsignedIntToolStripMenuItem.Text = "unsigned int";
            this.unsignedIntToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // unsignedLongIntToolStripMenuItem
            // 
            this.unsignedLongIntToolStripMenuItem.Name = "unsignedLongIntToolStripMenuItem";
            this.unsignedLongIntToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
            this.unsignedLongIntToolStripMenuItem.Text = "unsigned long int";
            this.unsignedLongIntToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // unsignedShortIntToolStripMenuItem
            // 
            this.unsignedShortIntToolStripMenuItem.Name = "unsignedShortIntToolStripMenuItem";
            this.unsignedShortIntToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
            this.unsignedShortIntToolStripMenuItem.Text = "unsigned short int";
            this.unsignedShortIntToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // unsignedLongLongIntToolStripMenuItem
            // 
            this.unsignedLongLongIntToolStripMenuItem.Name = "unsignedLongLongIntToolStripMenuItem";
            this.unsignedLongLongIntToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
            this.unsignedLongLongIntToolStripMenuItem.Text = "unsigned long long int";
            this.unsignedLongLongIntToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // floatingPointNumberToolStripMenuItem
            // 
            this.floatingPointNumberToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.floatToolStripMenuItem,
            this.doubleToolStripMenuItem,
            this.longDoubleToolStripMenuItem});
            this.floatingPointNumberToolStripMenuItem.Name = "floatingPointNumberToolStripMenuItem";
            this.floatingPointNumberToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.floatingPointNumberToolStripMenuItem.Text = "Floating point";
            // 
            // floatToolStripMenuItem
            // 
            this.floatToolStripMenuItem.Name = "floatToolStripMenuItem";
            this.floatToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.floatToolStripMenuItem.Text = "float";
            this.floatToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // doubleToolStripMenuItem
            // 
            this.doubleToolStripMenuItem.Name = "doubleToolStripMenuItem";
            this.doubleToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.doubleToolStripMenuItem.Text = "double";
            this.doubleToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // longDoubleToolStripMenuItem
            // 
            this.longDoubleToolStripMenuItem.Name = "longDoubleToolStripMenuItem";
            this.longDoubleToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.longDoubleToolStripMenuItem.Text = "long double";
            this.longDoubleToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // voidToolStripMenuItem
            // 
            this.voidToolStripMenuItem.Name = "voidToolStripMenuItem";
            this.voidToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.voidToolStripMenuItem.Text = "void";
            this.voidToolStripMenuItem.Click += new System.EventHandler(this.AddFundamentalTypeOnClick);
            // 
            // pointerToolStripMenuItem
            // 
            this.pointerToolStripMenuItem.Name = "pointerToolStripMenuItem";
            this.pointerToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.pointerToolStripMenuItem.Text = "Pointer";
            this.pointerToolStripMenuItem.Click += new System.EventHandler(this.AddTypeOnClick);
            // 
            // arrayToolStripMenuItem
            // 
            this.arrayToolStripMenuItem.Name = "arrayToolStripMenuItem";
            this.arrayToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.arrayToolStripMenuItem.Text = "Array";
            this.arrayToolStripMenuItem.Click += new System.EventHandler(this.AddTypeOnClick);
            // 
            // functionToolStripMenuItem
            // 
            this.functionToolStripMenuItem.Name = "functionToolStripMenuItem";
            this.functionToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.functionToolStripMenuItem.Text = "Function";
            this.functionToolStripMenuItem.Click += new System.EventHandler(this.AddFunctionOnClick);
            // 
            // customToolStripMenuItem
            // 
            this.customToolStripMenuItem.Name = "customToolStripMenuItem";
            this.customToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.customToolStripMenuItem.Text = "Custom";
            this.customToolStripMenuItem.Click += new System.EventHandler(this.AddTypeOnClick);
            // 
            // variadicToolStripMenuItem
            // 
            this.variadicToolStripMenuItem.Enabled = false;
            this.variadicToolStripMenuItem.Name = "variadicToolStripMenuItem";
            this.variadicToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.variadicToolStripMenuItem.Text = "Variadic";
            this.variadicToolStripMenuItem.Click += new System.EventHandler(this.AddVariadicOnClick);
            // 
            // removeNodeButton
            // 
            this.removeNodeButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.removeNodeButton.Enabled = false;
            this.removeNodeButton.Image = ((System.Drawing.Image)(resources.GetObject("removeNodeButton.Image")));
            this.removeNodeButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.removeNodeButton.Name = "removeNodeButton";
            this.removeNodeButton.Size = new System.Drawing.Size(84, 22);
            this.removeNodeButton.Text = "Remove node";
            this.removeNodeButton.Click += new System.EventHandler(this.RemoveNode);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripLabel1.ForeColor = System.Drawing.Color.DarkRed;
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(0, 22);
            // 
            // treeView1
            // 
            this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView1.Location = new System.Drawing.Point(0, 25);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(800, 425);
            this.treeView1.TabIndex = 1;
            this.treeView1.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.BeforeSelectNode);
            // 
            // Wizard
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.treeView1);
            this.Controls.Add(this.toolStrip1);
            this.MinimumSize = new System.Drawing.Size(300, 200);
            this.Name = "Wizard";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Declaration Designer";
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private ToolStrip toolStrip1;
        private TreeView treeView1;
        private ToolStripDropDownButton addNodeButton;
        private ToolStripMenuItem primitiveToolStripMenuItem;
        private ToolStripMenuItem pointerToolStripMenuItem;
        private ToolStripMenuItem arrayToolStripMenuItem;
        private ToolStripMenuItem functionToolStripMenuItem;
        private ToolStripMenuItem customToolStripMenuItem;
        private ToolStripButton removeNodeButton;
        private ToolStripMenuItem characterToolStripMenuItem;
        private ToolStripMenuItem charToolStripMenuItem;
        private ToolStripMenuItem signedCharToolStripMenuItem;
        private ToolStripMenuItem unsignedCharToolStripMenuItem;
        private ToolStripMenuItem integerToolStripMenuItem;
        private ToolStripMenuItem signedToolStripMenuItem;
        private ToolStripMenuItem intToolStripMenuItem;
        private ToolStripMenuItem shortIntToolStripMenuItem;
        private ToolStripMenuItem longIntToolStripMenuItem;
        private ToolStripMenuItem longLongIntToolStripMenuItem;
        private ToolStripMenuItem unsignedToolStripMenuItem;
        private ToolStripMenuItem unsignedIntToolStripMenuItem;
        private ToolStripMenuItem unsignedShortIntToolStripMenuItem;
        private ToolStripMenuItem unsignedLongIntToolStripMenuItem;
        private ToolStripMenuItem unsignedLongLongIntToolStripMenuItem;
        private ToolStripMenuItem floatingPointNumberToolStripMenuItem;
        private ToolStripMenuItem floatToolStripMenuItem;
        private ToolStripMenuItem doubleToolStripMenuItem;
        private ToolStripMenuItem longDoubleToolStripMenuItem;
        private ToolStripMenuItem voidToolStripMenuItem;
        private ToolStripMenuItem variadicToolStripMenuItem;
        private ToolStripMenuItem declarationToolStripMenuItem;
        private ToolStripDropDownButton declarationButton;
        private ToolStripMenuItem commitToolStripMenuItem;
        private ToolStripMenuItem reviewToolStripMenuItem;
        private ToolStripLabel toolStripLabel1;
    }
}