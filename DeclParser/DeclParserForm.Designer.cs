namespace DeclParser
{
    partial class DeclParserForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.parseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.textBoxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.userInputpermissiveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.wizardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripComboBox1 = new System.Windows.Forms.ToolStripComboBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.parserMsg = new System.Windows.Forms.ToolStripStatusLabel();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.textBox1.Location = new System.Drawing.Point(0, 24);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(605, 294);
            this.textBox1.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.AutoSize = false;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.parseToolStripMenuItem,
            this.wizardToolStripMenuItem,
            this.toolStripComboBox1});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(605, 24);
            this.menuStrip1.TabIndex = 7;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // parseToolStripMenuItem
            // 
            this.parseToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.textBoxToolStripMenuItem,
            this.userInputpermissiveToolStripMenuItem,
            this.fileToolStripMenuItem});
            this.parseToolStripMenuItem.Name = "parseToolStripMenuItem";
            this.parseToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.parseToolStripMenuItem.Text = "Parse";
            // 
            // textBoxToolStripMenuItem
            // 
            this.textBoxToolStripMenuItem.Name = "textBoxToolStripMenuItem";
            this.textBoxToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.textBoxToolStripMenuItem.Text = "User input (discrete)";
            this.textBoxToolStripMenuItem.ToolTipText = "Parses declarations only; other statements (e.g. loops, arithmetics) count as err" +
    "ors.";
            this.textBoxToolStripMenuItem.Click += new System.EventHandler(this.ParseClick);
            // 
            // userInputpermissiveToolStripMenuItem
            // 
            this.userInputpermissiveToolStripMenuItem.Name = "userInputpermissiveToolStripMenuItem";
            this.userInputpermissiveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.userInputpermissiveToolStripMenuItem.Text = "User input";
            this.userInputpermissiveToolStripMenuItem.ToolTipText = "Parses user-defined declarations while ignoring other statements (so are headers " +
    "and macros).";
            this.userInputpermissiveToolStripMenuItem.Click += new System.EventHandler(this.ParseClick);
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.fileToolStripMenuItem.Text = "C source file";
            this.fileToolStripMenuItem.ToolTipText = "Parses user-defined declarations while ignoring other statements (so are headers " +
    "and macros).";
            this.fileToolStripMenuItem.Click += new System.EventHandler(this.ParseClickFile);
            // 
            // wizardToolStripMenuItem
            // 
            this.wizardToolStripMenuItem.Name = "wizardToolStripMenuItem";
            this.wizardToolStripMenuItem.Size = new System.Drawing.Size(65, 20);
            this.wizardToolStripMenuItem.Text = "Designer";
            this.wizardToolStripMenuItem.Click += new System.EventHandler(this.WizardClick);
            // 
            // toolStripComboBox1
            // 
            this.toolStripComboBox1.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripComboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.toolStripComboBox1.Items.AddRange(new object[] {
            "LP32",
            "ILP32",
            "LLP64",
            "LP64"});
            this.toolStripComboBox1.Name = "toolStripComboBox1";
            this.toolStripComboBox1.Size = new System.Drawing.Size(121, 20);
            this.toolStripComboBox1.ToolTipText = "Data model";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.parserMsg});
            this.statusStrip1.Location = new System.Drawing.Point(0, 318);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.ShowItemToolTips = true;
            this.statusStrip1.Size = new System.Drawing.Size(605, 22);
            this.statusStrip1.TabIndex = 8;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // parserMsg
            // 
            this.parserMsg.AutoToolTip = true;
            this.parserMsg.Name = "parserMsg";
            this.parserMsg.Size = new System.Drawing.Size(590, 17);
            this.parserMsg.Spring = true;
            this.parserMsg.TextAlign = System.Drawing.ContentAlignment.TopLeft;
            // 
            // DeclParserForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(605, 340);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.MinimumSize = new System.Drawing.Size(300, 130);
            this.Name = "DeclParserForm";
            this.Text = "C Declaration Parser";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TextBox textBox1;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem parseToolStripMenuItem;
        private ToolStripMenuItem wizardToolStripMenuItem;
        private ToolStripComboBox toolStripComboBox1;
        private StatusStrip statusStrip1;
        private ToolStripStatusLabel parserMsg;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem textBoxToolStripMenuItem;
        private ToolStripMenuItem userInputpermissiveToolStripMenuItem;
    }
}