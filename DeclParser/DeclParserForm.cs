using System.Text.RegularExpressions;

namespace DeclParser
{
    public partial class DeclParserForm : Form
    {
        public DeclParserForm()
        {
            InitializeComponent();
            toolStripComboBox1.SelectedIndex = 0;
        }

        private void ParseClick(object sender, EventArgs e)
        {
            try
            {
                new Summary(new(textBox1.Text, (BaseType.DataModel)toolStripComboBox1.SelectedIndex,
                    sender == userInputpermissiveToolStripMenuItem)).Show();

                parserMsg.Text = string.Empty;
            }
            catch (Exception ex) when (ex is FormatException or ArgumentException)
            {
                parserMsg.Text = ex.Message;
                var match = PositionRegex().Match(ex.Message);

                if (match.Success)
                {
                    textBox1.Select(int.Parse(match.Groups[1].Value), int.Parse(match.Groups[2].Value));
                    textBox1.Focus();
                    textBox1.ScrollToCaret();
                }
            }
        }

        [GeneratedRegex(@"(?:At position )(\d+)(?: \(length )(\d+)")]
        private static partial Regex PositionRegex();

        private void WizardClick(object sender, EventArgs e)
        {
            Wizard w = new((BaseType.DataModel)toolStripComboBox1.SelectedIndex);
            w.ShowDialog();

            if (w.DialogResult == DialogResult.OK)
                textBox1.SelectedText = w.Buffer;
        }

        private void ParseClickFile(object sender, EventArgs e)
        {
            using OpenFileDialog ofd = new() { Filter = "C source file|*.c;*.h|C++ source file|*.cc;*.cxx;*.cpp;*.hpp", Multiselect = true };

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                string? text = null;
                
                try
                {
                    foreach (var fileName in ofd.FileNames)
                    {
                        text = File.ReadAllText(fileName);

                        new Summary(new(text, (BaseType.DataModel)toolStripComboBox1.SelectedIndex, true))
                        {
                            Text = $"Summary - {fileName}"
                        }.Show();
                    }

                    parserMsg.Text = string.Empty;
                }
                catch (Exception ex) when (ex is FormatException or ArgumentException)
                {
                    parserMsg.Text = ex.Message;
                    textBox1.Text = text;
                    var match = PositionRegex().Match(ex.Message);

                    if (match.Success)
                    {
                        textBox1.Select(int.Parse(match.Groups[1].Value), int.Parse(match.Groups[2].Value));
                        textBox1.Focus();
                        textBox1.ScrollToCaret();
                    }
                }
            }
        }
    }
}