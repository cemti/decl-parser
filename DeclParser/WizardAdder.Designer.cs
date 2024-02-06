namespace DeclParser
{
    partial class WizardAdder
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
            tableLayoutPanel1 = new TableLayoutPanel();
            panel1 = new Panel();
            button1 = new Button();
            wizardTabControl = new TabControl();
            declTab = new TabPage();
            groupBox1 = new GroupBox();
            registerRB = new RadioButton();
            inlineCB = new CheckBox();
            autoRB = new RadioButton();
            externRB = new RadioButton();
            staticRB = new RadioButton();
            declName = new TextBox();
            label1 = new Label();
            priTab = new TabPage();
            groupBox2 = new GroupBox();
            checkBox4 = new CheckBox();
            checkBox2 = new CheckBox();
            checkBox3 = new CheckBox();
            ptrTab = new TabPage();
            ptrView = new DataGridView();
            constC = new DataGridViewCheckBoxColumn();
            volatileC = new DataGridViewCheckBoxColumn();
            restrictC = new DataGridViewCheckBoxColumn();
            arrayTab = new TabPage();
            arrayView = new DataGridView();
            Count = new DataGridViewTextBoxColumn();
            suTab = new TabPage();
            definedCB = new CheckBox();
            groupBox3 = new GroupBox();
            structRB = new RadioButton();
            enumRB = new RadioButton();
            unionRB = new RadioButton();
            suName = new TextBox();
            label3 = new Label();
            tableLayoutPanel1.SuspendLayout();
            panel1.SuspendLayout();
            wizardTabControl.SuspendLayout();
            declTab.SuspendLayout();
            groupBox1.SuspendLayout();
            priTab.SuspendLayout();
            groupBox2.SuspendLayout();
            ptrTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)ptrView).BeginInit();
            arrayTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)arrayView).BeginInit();
            suTab.SuspendLayout();
            groupBox3.SuspendLayout();
            SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.ColumnCount = 1;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.Controls.Add(panel1, 0, 1);
            tableLayoutPanel1.Controls.Add(wizardTabControl, 0, 0);
            tableLayoutPanel1.Dock = DockStyle.Fill;
            tableLayoutPanel1.Location = new Point(0, 0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 2;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 77.82609F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 22.17391F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Absolute, 20F));
            tableLayoutPanel1.Size = new Size(234, 211);
            tableLayoutPanel1.TabIndex = 0;
            // 
            // panel1
            // 
            panel1.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            panel1.Controls.Add(button1);
            panel1.Location = new Point(3, 167);
            panel1.Name = "panel1";
            panel1.Size = new Size(228, 41);
            panel1.TabIndex = 0;
            // 
            // button1
            // 
            button1.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            button1.Location = new Point(144, 9);
            button1.Name = "button1";
            button1.Size = new Size(75, 23);
            button1.TabIndex = 0;
            button1.Text = "OK";
            button1.UseVisualStyleBackColor = true;
            button1.Click += Commit;
            // 
            // wizardTabControl
            // 
            wizardTabControl.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            wizardTabControl.Controls.Add(declTab);
            wizardTabControl.Controls.Add(priTab);
            wizardTabControl.Controls.Add(ptrTab);
            wizardTabControl.Controls.Add(arrayTab);
            wizardTabControl.Controls.Add(suTab);
            wizardTabControl.Location = new Point(3, 3);
            wizardTabControl.Name = "wizardTabControl";
            wizardTabControl.SelectedIndex = 0;
            wizardTabControl.Size = new Size(228, 158);
            wizardTabControl.TabIndex = 1;
            // 
            // declTab
            // 
            declTab.Controls.Add(groupBox1);
            declTab.Controls.Add(declName);
            declTab.Controls.Add(label1);
            declTab.Location = new Point(4, 24);
            declTab.Name = "declTab";
            declTab.Padding = new Padding(3);
            declTab.Size = new Size(220, 130);
            declTab.TabIndex = 0;
            declTab.Text = "Declaration";
            declTab.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(registerRB);
            groupBox1.Controls.Add(inlineCB);
            groupBox1.Controls.Add(autoRB);
            groupBox1.Controls.Add(externRB);
            groupBox1.Controls.Add(staticRB);
            groupBox1.Location = new Point(15, 44);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(191, 72);
            groupBox1.TabIndex = 3;
            groupBox1.TabStop = false;
            groupBox1.Text = "Specifiers";
            // 
            // registerRB
            // 
            registerRB.AutoSize = true;
            registerRB.Location = new Point(6, 46);
            registerRB.Name = "registerRB";
            registerRB.Size = new Size(64, 19);
            registerRB.TabIndex = 7;
            registerRB.TabStop = true;
            registerRB.Text = "register";
            registerRB.UseVisualStyleBackColor = true;
            registerRB.CheckedChanged += ModifySpecifier;
            // 
            // inlineCB
            // 
            inlineCB.AutoSize = true;
            inlineCB.Location = new Point(76, 46);
            inlineCB.Name = "inlineCB";
            inlineCB.Size = new Size(55, 19);
            inlineCB.TabIndex = 2;
            inlineCB.Text = "inline";
            inlineCB.UseVisualStyleBackColor = true;
            // 
            // autoRB
            // 
            autoRB.AutoSize = true;
            autoRB.Location = new Point(6, 22);
            autoRB.Name = "autoRB";
            autoRB.Size = new Size(49, 19);
            autoRB.TabIndex = 4;
            autoRB.TabStop = true;
            autoRB.Text = "auto";
            autoRB.UseVisualStyleBackColor = true;
            autoRB.CheckedChanged += ModifySpecifier;
            // 
            // externRB
            // 
            externRB.AutoSize = true;
            externRB.Location = new Point(120, 22);
            externRB.Name = "externRB";
            externRB.Size = new Size(58, 19);
            externRB.TabIndex = 6;
            externRB.TabStop = true;
            externRB.Text = "extern";
            externRB.UseVisualStyleBackColor = true;
            externRB.CheckedChanged += ModifySpecifier;
            // 
            // staticRB
            // 
            staticRB.AutoSize = true;
            staticRB.Location = new Point(61, 22);
            staticRB.Name = "staticRB";
            staticRB.Size = new Size(53, 19);
            staticRB.TabIndex = 5;
            staticRB.TabStop = true;
            staticRB.Text = "static";
            staticRB.UseVisualStyleBackColor = true;
            staticRB.CheckedChanged += ModifySpecifier;
            // 
            // declName
            // 
            declName.Location = new Point(63, 15);
            declName.Name = "declName";
            declName.Size = new Size(143, 23);
            declName.TabIndex = 1;
            declName.Tag = "";
            declName.TextChanged += OnNameInput;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(15, 18);
            label1.Name = "label1";
            label1.Size = new Size(42, 15);
            label1.TabIndex = 0;
            label1.Text = "Name:";
            // 
            // priTab
            // 
            priTab.Controls.Add(groupBox2);
            priTab.Location = new Point(4, 24);
            priTab.Name = "priTab";
            priTab.Size = new Size(220, 130);
            priTab.TabIndex = 1;
            priTab.Text = "Qualifiers";
            priTab.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(checkBox4);
            groupBox2.Controls.Add(checkBox2);
            groupBox2.Controls.Add(checkBox3);
            groupBox2.Location = new Point(10, 16);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(200, 52);
            groupBox2.TabIndex = 0;
            groupBox2.TabStop = false;
            groupBox2.Text = "Qualifiers";
            // 
            // checkBox4
            // 
            checkBox4.AutoSize = true;
            checkBox4.Location = new Point(137, 22);
            checkBox4.Name = "checkBox4";
            checkBox4.Size = new Size(62, 19);
            checkBox4.TabIndex = 3;
            checkBox4.Text = "restrict";
            checkBox4.UseVisualStyleBackColor = true;
            checkBox4.CheckedChanged += ModifyQualifiers;
            // 
            // checkBox2
            // 
            checkBox2.AutoSize = true;
            checkBox2.Location = new Point(6, 22);
            checkBox2.Name = "checkBox2";
            checkBox2.Size = new Size(55, 19);
            checkBox2.TabIndex = 1;
            checkBox2.Text = "const";
            checkBox2.UseVisualStyleBackColor = true;
            checkBox2.CheckedChanged += ModifyQualifiers;
            // 
            // checkBox3
            // 
            checkBox3.AutoSize = true;
            checkBox3.Location = new Point(67, 22);
            checkBox3.Name = "checkBox3";
            checkBox3.Size = new Size(64, 19);
            checkBox3.TabIndex = 2;
            checkBox3.Text = "volatile";
            checkBox3.UseVisualStyleBackColor = true;
            checkBox3.CheckedChanged += ModifyQualifiers;
            // 
            // ptrTab
            // 
            ptrTab.Controls.Add(ptrView);
            ptrTab.Location = new Point(4, 24);
            ptrTab.Name = "ptrTab";
            ptrTab.Size = new Size(220, 130);
            ptrTab.TabIndex = 6;
            ptrTab.Text = "Pointer";
            ptrTab.UseVisualStyleBackColor = true;
            // 
            // ptrView
            // 
            ptrView.AllowUserToResizeColumns = false;
            ptrView.AllowUserToResizeRows = false;
            ptrView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            ptrView.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
            ptrView.BackgroundColor = SystemColors.Control;
            ptrView.BorderStyle = BorderStyle.None;
            ptrView.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            ptrView.Columns.AddRange(new DataGridViewColumn[] { constC, volatileC, restrictC });
            ptrView.Dock = DockStyle.Fill;
            ptrView.Location = new Point(0, 0);
            ptrView.Name = "ptrView";
            ptrView.RowHeadersWidth = 24;
            ptrView.RowHeadersWidthSizeMode = DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            ptrView.RowTemplate.Height = 25;
            ptrView.Size = new Size(220, 130);
            ptrView.TabIndex = 2;
            // 
            // constC
            // 
            constC.HeaderText = "const";
            constC.Name = "constC";
            // 
            // volatileC
            // 
            volatileC.HeaderText = "volatile";
            volatileC.Name = "volatileC";
            // 
            // restrictC
            // 
            restrictC.HeaderText = "restrict";
            restrictC.Name = "restrictC";
            // 
            // arrayTab
            // 
            arrayTab.Controls.Add(arrayView);
            arrayTab.Location = new Point(4, 24);
            arrayTab.Name = "arrayTab";
            arrayTab.Size = new Size(220, 130);
            arrayTab.TabIndex = 3;
            arrayTab.Text = "Array";
            arrayTab.UseVisualStyleBackColor = true;
            // 
            // arrayView
            // 
            arrayView.AllowUserToResizeColumns = false;
            arrayView.AllowUserToResizeRows = false;
            arrayView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            arrayView.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
            arrayView.BackgroundColor = SystemColors.Control;
            arrayView.BorderStyle = BorderStyle.None;
            arrayView.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            arrayView.Columns.AddRange(new DataGridViewColumn[] { Count });
            arrayView.Dock = DockStyle.Fill;
            arrayView.Location = new Point(0, 0);
            arrayView.Name = "arrayView";
            arrayView.RowHeadersWidth = 24;
            arrayView.RowHeadersWidthSizeMode = DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            arrayView.Size = new Size(220, 130);
            arrayView.TabIndex = 3;
            // 
            // Count
            // 
            Count.HeaderText = "Count";
            Count.Name = "Count";
            Count.SortMode = DataGridViewColumnSortMode.NotSortable;
            // 
            // suTab
            // 
            suTab.Controls.Add(definedCB);
            suTab.Controls.Add(groupBox3);
            suTab.Controls.Add(suName);
            suTab.Controls.Add(label3);
            suTab.Location = new Point(4, 24);
            suTab.Name = "suTab";
            suTab.Padding = new Padding(3);
            suTab.Size = new Size(220, 130);
            suTab.TabIndex = 5;
            suTab.Text = "Custom";
            suTab.UseVisualStyleBackColor = true;
            // 
            // definedCB
            // 
            definedCB.AutoSize = true;
            definedCB.Checked = true;
            definedCB.CheckState = CheckState.Checked;
            definedCB.Location = new Point(15, 98);
            definedCB.Name = "definedCB";
            definedCB.Size = new Size(131, 19);
            definedCB.TabIndex = 7;
            definedCB.Text = "Forward declaration";
            definedCB.UseVisualStyleBackColor = true;
            definedCB.CheckedChanged += ToggleFwdDecl;
            // 
            // groupBox3
            // 
            groupBox3.Controls.Add(structRB);
            groupBox3.Controls.Add(enumRB);
            groupBox3.Controls.Add(unionRB);
            groupBox3.Location = new Point(15, 42);
            groupBox3.Name = "groupBox3";
            groupBox3.Size = new Size(191, 50);
            groupBox3.TabIndex = 6;
            groupBox3.TabStop = false;
            groupBox3.Text = "Type";
            // 
            // structRB
            // 
            structRB.AutoSize = true;
            structRB.Checked = true;
            structRB.Location = new Point(6, 22);
            structRB.Name = "structRB";
            structRB.Size = new Size(55, 19);
            structRB.TabIndex = 4;
            structRB.TabStop = true;
            structRB.Text = "struct";
            structRB.UseVisualStyleBackColor = true;
            // 
            // enumRB
            // 
            enumRB.AutoSize = true;
            enumRB.Location = new Point(129, 22);
            enumRB.Name = "enumRB";
            enumRB.Size = new Size(56, 19);
            enumRB.TabIndex = 5;
            enumRB.Text = "enum";
            enumRB.UseVisualStyleBackColor = true;
            // 
            // unionRB
            // 
            unionRB.AutoSize = true;
            unionRB.Location = new Point(67, 22);
            unionRB.Name = "unionRB";
            unionRB.Size = new Size(56, 19);
            unionRB.TabIndex = 5;
            unionRB.Text = "union";
            unionRB.UseVisualStyleBackColor = true;
            // 
            // suName
            // 
            suName.Location = new Point(63, 13);
            suName.Name = "suName";
            suName.Size = new Size(143, 23);
            suName.TabIndex = 3;
            suName.TextChanged += OnNameInput;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(15, 16);
            label3.Name = "label3";
            label3.Size = new Size(42, 15);
            label3.TabIndex = 2;
            label3.Text = "Name:";
            // 
            // WizardAdder
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(234, 211);
            Controls.Add(tableLayoutPanel1);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            Name = "WizardAdder";
            ShowInTaskbar = false;
            StartPosition = FormStartPosition.CenterParent;
            Text = "Add Node";
            tableLayoutPanel1.ResumeLayout(false);
            panel1.ResumeLayout(false);
            wizardTabControl.ResumeLayout(false);
            declTab.ResumeLayout(false);
            declTab.PerformLayout();
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            priTab.ResumeLayout(false);
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            ptrTab.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)ptrView).EndInit();
            arrayTab.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)arrayView).EndInit();
            suTab.ResumeLayout(false);
            suTab.PerformLayout();
            groupBox3.ResumeLayout(false);
            groupBox3.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private TableLayoutPanel tableLayoutPanel1;
        private Panel panel1;
        private Button button1;
        private TabControl wizardTabControl;
        private TabPage declTab;
        private GroupBox groupBox1;
        private RadioButton registerRB;
        private CheckBox inlineCB;
        private RadioButton autoRB;
        private RadioButton externRB;
        private RadioButton staticRB;
        private TextBox declName;
        private Label label1;
        private TabPage priTab;
        private GroupBox groupBox2;
        private CheckBox checkBox4;
        private CheckBox checkBox2;
        private CheckBox checkBox3;
        private TabPage arrayTab;
        private TabPage suTab;
        private RadioButton enumRB;
        private RadioButton unionRB;
        private RadioButton structRB;
        private TextBox suName;
        private Label label3;
        private GroupBox groupBox3;
        private CheckBox definedCB;
        private DataGridView arrayView;
        private DataGridViewTextBoxColumn Count;
        private TabPage ptrTab;
        private DataGridView ptrView;
        private DataGridViewCheckBoxColumn constC;
        private DataGridViewCheckBoxColumn volatileC;
        private DataGridViewCheckBoxColumn restrictC;
    }
}