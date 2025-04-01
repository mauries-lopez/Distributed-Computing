namespace Project
{
    partial class Producer
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
            logBox = new TextBox();
            numThreadsInput = new TextBox();
            threadInputLabel = new Label();
            mainBtn = new Button();
            browseBtn = new Button();
            selectedFileLog = new TextBox();
            selectedFileLabel = new Label();
            folderBrowserDialog1 = new FolderBrowserDialog();
            connectedClientList = new CheckedListBox();
            connectedClientLabel = new Label();
            progressBar = new ProgressBar();
            SuspendLayout();
            // 
            // logBox
            // 
            logBox.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            logBox.Location = new Point(422, 101);
            logBox.Multiline = true;
            logBox.Name = "logBox";
            logBox.ReadOnly = true;
            logBox.ScrollBars = ScrollBars.Vertical;
            logBox.Size = new Size(366, 337);
            logBox.TabIndex = 0;
            logBox.TabStop = false;
            // 
            // numThreadsInput
            // 
            numThreadsInput.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            numThreadsInput.Location = new Point(12, 36);
            numThreadsInput.Name = "numThreadsInput";
            numThreadsInput.Size = new Size(397, 23);
            numThreadsInput.TabIndex = 1;
            numThreadsInput.WordWrap = false;
            // 
            // threadInputLabel
            // 
            threadInputLabel.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            threadInputLabel.AutoSize = true;
            threadInputLabel.Font = new Font("Segoe UI", 12F);
            threadInputLabel.Location = new Point(12, 12);
            threadInputLabel.Name = "threadInputLabel";
            threadInputLabel.Size = new Size(212, 21);
            threadInputLabel.TabIndex = 2;
            threadInputLabel.Text = "Number of Producer Threads";
            threadInputLabel.TextAlign = ContentAlignment.MiddleLeft;
            // 
            // mainBtn
            // 
            mainBtn.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            mainBtn.FlatStyle = FlatStyle.Flat;
            mainBtn.Font = new Font("Segoe UI", 15F, FontStyle.Bold);
            mainBtn.Location = new Point(12, 381);
            mainBtn.Name = "mainBtn";
            mainBtn.Size = new Size(397, 57);
            mainBtn.TabIndex = 4;
            mainBtn.Text = "INITIALIZE";
            mainBtn.UseVisualStyleBackColor = true;
            mainBtn.Click += mainBtn_Click;
            // 
            // browseBtn
            // 
            browseBtn.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            browseBtn.Location = new Point(12, 36);
            browseBtn.Name = "browseBtn";
            browseBtn.Size = new Size(397, 23);
            browseBtn.TabIndex = 5;
            browseBtn.Text = "Browse";
            browseBtn.UseVisualStyleBackColor = true;
            browseBtn.Visible = false;
            browseBtn.Click += browseBtn_Click;
            // 
            // selectedFileLog
            // 
            selectedFileLog.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            selectedFileLog.Location = new Point(12, 82);
            selectedFileLog.Multiline = true;
            selectedFileLog.Name = "selectedFileLog";
            selectedFileLog.ReadOnly = true;
            selectedFileLog.ScrollBars = ScrollBars.Both;
            selectedFileLog.ShortcutsEnabled = false;
            selectedFileLog.Size = new Size(397, 272);
            selectedFileLog.TabIndex = 6;
            selectedFileLog.TabStop = false;
            selectedFileLog.Visible = false;
            selectedFileLog.WordWrap = false;
            // 
            // selectedFileLabel
            // 
            selectedFileLabel.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            selectedFileLabel.AutoSize = true;
            selectedFileLabel.Font = new Font("Segoe UI", 9F, FontStyle.Italic);
            selectedFileLabel.Location = new Point(12, 64);
            selectedFileLabel.Name = "selectedFileLabel";
            selectedFileLabel.Size = new Size(95, 15);
            selectedFileLabel.TabIndex = 7;
            selectedFileLabel.Text = "Selected Folder/s";
            selectedFileLabel.TextAlign = ContentAlignment.MiddleLeft;
            selectedFileLabel.Visible = false;
            // 
            // connectedClientList
            // 
            connectedClientList.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            connectedClientList.BackColor = SystemColors.Control;
            connectedClientList.FormattingEnabled = true;
            connectedClientList.Location = new Point(422, 37);
            connectedClientList.Name = "connectedClientList";
            connectedClientList.Size = new Size(366, 58);
            connectedClientList.TabIndex = 8;
            connectedClientList.ThreeDCheckBoxes = true;
            connectedClientList.SelectedIndexChanged += connectedClientList_SelectedIndexChanged;
            // 
            // connectedClientLabel
            // 
            connectedClientLabel.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            connectedClientLabel.AutoSize = true;
            connectedClientLabel.Font = new Font("Segoe UI", 12F);
            connectedClientLabel.Location = new Point(422, 12);
            connectedClientLabel.Name = "connectedClientLabel";
            connectedClientLabel.Size = new Size(176, 21);
            connectedClientLabel.TabIndex = 9;
            connectedClientLabel.Text = "Connected Consumer/s:";
            connectedClientLabel.TextAlign = ContentAlignment.MiddleLeft;
            // 
            // progressBar
            // 
            progressBar.Location = new Point(12, 381);
            progressBar.Name = "progressBar";
            progressBar.Size = new Size(397, 57);
            progressBar.TabIndex = 10;
            progressBar.Visible = false;
            // 
            // Producer
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(progressBar);
            Controls.Add(connectedClientLabel);
            Controls.Add(connectedClientList);
            Controls.Add(selectedFileLabel);
            Controls.Add(selectedFileLog);
            Controls.Add(browseBtn);
            Controls.Add(mainBtn);
            Controls.Add(threadInputLabel);
            Controls.Add(numThreadsInput);
            Controls.Add(logBox);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            KeyPreview = true;
            MaximizeBox = false;
            Name = "Producer";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Producer";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox logBox;
        private TextBox numThreadsInput;
        private Label threadInputLabel;
        private Button mainBtn;
        private Button browseBtn;
        private TextBox selectedFileLog;
        private Label selectedFileLabel;
        private FolderBrowserDialog folderBrowserDialog1;
        private CheckedListBox connectedClientList;
        private Label connectedClientLabel;
        private ProgressBar progressBar;
    }
}
