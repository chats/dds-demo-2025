function showAlert(type, duration = 3000, message = null) {
    const alertElement = document.getElementById(type + 'Alert');
    if (message) {
        alertElement.textContent = message;
    }
    alertElement.style.display = 'block';
    setTimeout(() => {
        alertElement.style.display = 'none';
        // Reset to default message
        if (type === 'success') {
            alertElement.textContent = 'Message published successfully!';
        } else {
            alertElement.textContent = 'Failed to publish message. Please try again.';
        }
    }, duration);
}

function handleFileSelect() {
    const fileInput = document.getElementById('fileInput');
    const preview = document.getElementById('uploadPreview');
    const fileInfo = preview.querySelector('.file-info');
    const progress = preview.querySelector('.progress');
    const progressBar = preview.querySelector('.progress-bar');
    const publishButton = document.getElementById('publishButton');

    if (fileInput.files.length > 0) {
        const file = fileInput.files[0];
        
        // Check file extension
        const extension = file.name.split('.').pop().toLowerCase();
        if (!['xlsx', 'xls'].includes(extension)) {
            showAlert('error', 5000, 'Please select an Excel file (.xlsx or .xls)');
            fileInput.value = '';  // Clear file input
            preview.style.display = 'none';
            return;
        }

        fileInfo.textContent = `Selected file: ${file.name} (${formatFileSize(file.size)})`;
        preview.style.display = 'block';
        progress.style.display = 'block';
        publishButton.disabled = true;

        const formData = new FormData();
        formData.append('file', file);

        fetch('/upload', {
            method: 'POST',
            body: formData
        })
        .then(response => response.json())
        .then(result => {
            if (result.fileUrl) {
                document.getElementById('fileUrl').value = result.fileUrl;
                progressBar.style.width = '100%';
                publishButton.disabled = false;
                showAlert('success', 3000, 'File uploaded successfully!');
            } else {
                throw new Error(result.error || 'Upload failed');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            showAlert('error', 5000, error.message || 'Failed to upload file');
            preview.style.display = 'none';
            publishButton.disabled = false;
            fileInput.value = '';  // Clear file input
        });
    }
}

function formatFileSize(bytes) {
    if (bytes === 0) return '0 Bytes';
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

function publishMessage() {
    const data = {
        subject: document.getElementById('subject').value,
        recipient: document.getElementById('recipient').value,
        fileUrl: document.getElementById('fileUrl').value
    };

    if (!data.subject || !data.recipient || !data.fileUrl) {
        showAlert('error', 3000, 'Please fill in all fields and upload an Excel file');
        return;
    }

    fetch('/publish', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    .then(response => response.json())
    .then(result => {
        if (result.status === 'success') {
            showAlert('success');
            // Clear form
            document.getElementById('subject').value = '';
            document.getElementById('recipient').value = '';
            document.getElementById('fileInput').value = '';
            document.getElementById('fileUrl').value = '';
            document.getElementById('uploadPreview').style.display = 'none';
        } else {
            throw new Error(result.message || 'Failed to publish');
        }
    })
    .catch(error => {
        console.error('Error:', error);
        showAlert('error', 5000, error.message || 'Failed to publish message');
    });
}