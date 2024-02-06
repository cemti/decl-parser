namespace DeclParser
{
    using static BaseType;
    using static Declaration;
    
    public partial class WizardAdder : Form
    {
        private readonly string? _name;
        private StorageSpecifier _specifier;
        private TypeQualifiers _qualifiers;
        private BaseType? _type;

        public object? Result { get; private set; }

        private TextBox CurrentTextBox =>
            wizardTabControl.Controls[0] == declTab
                ? declName
                : wizardTabControl.Controls[0] == suTab
                ? suName
                : throw new InvalidOperationException();

        public string Placeholder
        {
            get => CurrentTextBox.PlaceholderText;
            set => CurrentTextBox.PlaceholderText = value;
        }

        public WizardAdder(string name, bool disableQ = false)
        {
            InitializeComponent();
            wizardTabControl.Controls.Clear();
            _name = name;

            switch (name)
            {
                case "Declaration":
                    wizardTabControl.Controls.Add(declTab);
                    groupBox1.Enabled = !disableQ;
                    break;

                case "Array":
                    arrayView.RowsAdded += GenericRowsAdded<ArrayType>;
                    arrayView.CellValueChanged += ArrayCellValueChanged;
                    arrayView.UserDeletingRow += GenericUserDeletingRow;
                    arrayView.CellMouseClick += GenericCellMouseClick;

                    wizardTabControl.Controls.Add(arrayTab);
                    button1.Enabled = false;
                    break;

                case "Pointer":
                    ptrView.RowsAdded += GenericRowsAdded<PointerType>;
                    ptrView.CellValueChanged += PointerCellValueChanged;
                    ptrView.UserDeletingRow += GenericUserDeletingRow;
                    ptrView.CellMouseClick += GenericCellMouseClick;

                    ptrView.Rows.Add();
                    wizardTabControl.Controls.Add(ptrTab);
                    break;

                case "Custom":
                    wizardTabControl.Controls.AddRange(new[] { suTab, priTab });
                    button1.Enabled = false;
                    break;

                default:
                    wizardTabControl.Controls.Add(priTab);
                    priTab.Text = name;
                    break;
            }
        }

        public WizardAdder(string declaration, DataModel dataModel) : this(declaration) =>
            _type = declaration == "void"
                ? new FundamentalType(FundamentalType.DataType.Void)
                : new TypeParser($"{declaration} _;", dataModel).Variables[0].Declaration.Type;

        private void OnNameInput(object sender, EventArgs e)
        {
            var textBox = (TextBox)sender;
            textBox.Text = textBox.Text.Trim();
            button1.Enabled = !(textBox.Text.Length == 0
                ? textBox == suName && definedCB.Checked
                : char.IsAsciiDigit(textBox.Text[0]));
        }

        private void Commit(object sender, EventArgs e)
        {
            switch (_name)
            {
                case "Declaration":
                    Result = new NamedDeclaration(declName.Text, new Declaration(null, _specifier, inlineCB.Checked));
                    break;

                case "Pointer":
                    Result = ptrView.Rows[0].Tag;
                    break;

                case "Array":
                    Result = arrayView.Rows[0].Tag;
                    break;

                case "Custom":
                    NamedType sType = enumRB.Checked
                        ? new EnumType(suName.Text, !definedCB.Checked)
                        : new StructType(suName.Text, unionRB.Checked, !definedCB.Checked);

                    if (sType.Anonymous)
                        sType.Name = "__custom_" + Guid.NewGuid().ToString().Replace("-", string.Empty);

                    _type = sType;
                    goto default;

                default:
                    _type?.SetQualifier(_qualifiers);
                    Result = _type;
                    break;
            }

            DialogResult = DialogResult.OK;
            Close();
        }

        private void ModifySpecifier(object sender, EventArgs e) =>
            _specifier ^= Enum.Parse<StorageSpecifier>(((RadioButton)sender).Text, true);

        private void ModifyQualifiers(object sender, EventArgs e) =>
            _qualifiers ^= Enum.Parse<TypeQualifiers>(((CheckBox)sender).Text, true);

        private void ToggleFwdDecl(object sender, EventArgs e) =>
            button1.Enabled = !((CheckBox)sender).Checked || suName.Text.Length > 0;

        private void ArrayCellValueChanged(object? sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0 && sender is DataGridView view)
            {
                var row = view.Rows[e.RowIndex];
                var cell = row.Cells[0];

                if (row.Tag is ArrayType aType)
                {
                    if (int.TryParse((string)cell.Value, out var count))
                        aType.Count = count;
                    else
                    {
                        cell.Value = "unspecified";
                        aType.Count = null;
                    }
                }
            }
        }
        private void PointerCellValueChanged(object? sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0 && sender is DataGridView view && view.Rows[e.RowIndex].Tag is PointerType pType)
                pType.Qualifiers ^= Enum.Parse<TypeQualifiers>(view.Columns[e.ColumnIndex].HeaderText, true);
        }

        private void GenericRowsAdded<T>(object? sender, DataGridViewRowsAddedEventArgs e) where T : CompositeType, new()
        {
            if (sender is DataGridView view)
            {
                var index = e.RowIndex;

                if (index + 1 == view.Rows.Count)
                    --index;

                T type = new();
                view.Rows[index].Tag = type;

                if (type is ArrayType)
                {
                    var cell = view[0, index];

                    if (string.IsNullOrWhiteSpace((string)cell.Value))
                        cell.Value = string.Empty;
                }

                if (index > 0 && view.Rows[index - 1].Tag is T aType)
                    aType.Decay = type;

                button1.Enabled = true;
            }
        }

        private void GenericUserDeletingRow(object? sender, DataGridViewRowCancelEventArgs e)
        {
            if (e.Row is not null && sender is DataGridView view)
            {
                if (e.Row.Index > 0 && view.Rows[e.Row.Index - 1].Tag is CompositeType aType)
                    aType.Decay = ((CompositeType?)view.Rows[e.Row.Index].Tag)?.Decay;

                button1.Enabled = view.Rows.Count > 2;
            }
        }

        private void GenericCellMouseClick(object? sender, DataGridViewCellMouseEventArgs e)
        {
            if (sender is DataGridView view && e.ColumnIndex == -1 && e.RowIndex + 1 == view.Rows.Count)
                view.Rows.Add();
        }
    }
}
