
namespace Project
{
    partial class Consumer
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
            components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Consumer));
            logBox = new TextBox();
            numThreadsInput = new TextBox();
            threadInputLabel = new Label();
            mainBtn = new Button();
            ipAddressLabel = new Label();
            ipAddressInput = new TextBox();
            maxQueueLengthLabel = new Label();
            maxQueueLengthInput = new TextBox();
            thumbnailListView = new ListView();
            thumbnailImageList = new ImageList(components);
            windowsMediaPlayer = new AxWMPLib.AxWindowsMediaPlayer();
            ((System.ComponentModel.ISupportInitialize)windowsMediaPlayer).BeginInit();
            SuspendLayout();
            // 
            // logBox
            // 
            logBox.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            logBox.Location = new Point(422, 12);
            logBox.Multiline = true;
            logBox.Name = "logBox";
            logBox.ReadOnly = true;
            logBox.ScrollBars = ScrollBars.Vertical;
            logBox.ShortcutsEnabled = false;
            logBox.Size = new Size(366, 426);
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
            threadInputLabel.Size = new Size(221, 21);
            threadInputLabel.TabIndex = 2;
            threadInputLabel.Text = "Number of Consumer Threads";
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
            // ipAddressLabel
            // 
            ipAddressLabel.AutoSize = true;
            ipAddressLabel.Font = new Font("Segoe UI", 12F);
            ipAddressLabel.Location = new Point(12, 132);
            ipAddressLabel.Name = "ipAddressLabel";
            ipAddressLabel.Size = new Size(154, 21);
            ipAddressLabel.TabIndex = 5;
            ipAddressLabel.Text = "IPv4 Address of Host";
            ipAddressLabel.TextAlign = ContentAlignment.MiddleLeft;
            // 
            // ipAddressInput
            // 
            ipAddressInput.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            ipAddressInput.Location = new Point(12, 156);
            ipAddressInput.Name = "ipAddressInput";
            ipAddressInput.Size = new Size(397, 23);
            ipAddressInput.TabIndex = 6;
            ipAddressInput.WordWrap = false;
            // 
            // maxQueueLengthLabel
            // 
            maxQueueLengthLabel.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            maxQueueLengthLabel.AutoSize = true;
            maxQueueLengthLabel.Font = new Font("Segoe UI", 12F);
            maxQueueLengthLabel.Location = new Point(12, 72);
            maxQueueLengthLabel.Name = "maxQueueLengthLabel";
            maxQueueLengthLabel.Size = new Size(141, 21);
            maxQueueLengthLabel.TabIndex = 7;
            maxQueueLengthLabel.Text = "Max Queue Length";
            maxQueueLengthLabel.TextAlign = ContentAlignment.MiddleLeft;
            // 
            // maxQueueLengthInput
            // 
            maxQueueLengthInput.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            maxQueueLengthInput.Location = new Point(12, 96);
            maxQueueLengthInput.Name = "maxQueueLengthInput";
            maxQueueLengthInput.Size = new Size(397, 23);
            maxQueueLengthInput.TabIndex = 8;
            maxQueueLengthInput.WordWrap = false;
            // 
            // thumbnailListView
            // 
            thumbnailListView.Activation = ItemActivation.OneClick;
            thumbnailListView.Alignment = ListViewAlignment.Default;
            thumbnailListView.HoverSelection = true;
            thumbnailListView.Location = new Point(12, 36);
            thumbnailListView.MultiSelect = false;
            thumbnailListView.Name = "thumbnailListView";
            thumbnailListView.Size = new Size(397, 93);
            thumbnailListView.TabIndex = 11;
            thumbnailListView.UseCompatibleStateImageBehavior = false;
            thumbnailListView.Visible = false;
            thumbnailListView.SelectedIndexChanged += thumbnailListView_SelectedIndexChanged;
            // 
            // thumbnailImageList
            // 
            thumbnailImageList.ColorDepth = ColorDepth.Depth32Bit;
            thumbnailImageList.ImageSize = new Size(16, 16);
            thumbnailImageList.TransparentColor = Color.Transparent;
            // 
            // windowsMediaPlayer
            // 
            windowsMediaPlayer.Enabled = true;
            windowsMediaPlayer.Location = new Point(12, 156);
            windowsMediaPlayer.Name = "windowsMediaPlayer";
            windowsMediaPlayer.OcxState = (AxHost.State)resources.GetObject("windowsMediaPlayer.OcxState");
            windowsMediaPlayer.Size = new Size(397, 282);
            windowsMediaPlayer.TabIndex = 12;
            windowsMediaPlayer.Visible = false;
            // 
            // Consumer
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(windowsMediaPlayer);
            Controls.Add(thumbnailListView);
            Controls.Add(maxQueueLengthInput);
            Controls.Add(maxQueueLengthLabel);
            Controls.Add(ipAddressInput);
            Controls.Add(ipAddressLabel);
            Controls.Add(mainBtn);
            Controls.Add(threadInputLabel);
            Controls.Add(numThreadsInput);
            Controls.Add(logBox);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            KeyPreview = true;
            MaximizeBox = false;
            Name = "Consumer";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Consumer";
            ((System.ComponentModel.ISupportInitialize)windowsMediaPlayer).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox logBox;
        private TextBox numThreadsInput;
        private Label threadInputLabel;
        private Button mainBtn;
        private Label ipAddressLabel;
        private TextBox ipAddressInput;
        private Label maxQueueLengthLabel;
        private TextBox maxQueueLengthInput;
        private ImageList thumbnailList;
        private ListView listView1;
        private ListView thumbnailListView;
        private ImageList thumbnailImageList;
        private AxWMPLib.AxWindowsMediaPlayer windowsMediaPlayer;
    }
}
