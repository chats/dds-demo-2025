function showAlert(type, duration = 3000, message = null) {
    const alertElement = document.getElementById(type + 'Alert');
    if (message) {
        alertElement.textContent = message;
    }
    alertElement.style.display = 'block';
    setTimeout(() => {
        alertElement.style.display = 'none';
        if (type === 'success') {
            alertElement.textContent = 'Message published successfully!';
        } else {
            alertElement.textContent = 'Failed to publish message. Please try again.';
        }
    }, duration);
}

async function handleFileSelect() {
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
            fileInput.value = '';
            preview.style.display = 'none';
            return;
        }

        fileInfo.textContent = `Selected file: ${file.name} (${formatFileSize(file.size)})`;
        preview.style.display = 'block';
        progress.style.display = 'block';
        publishButton.disabled = true;

        try {
            const formData = new FormData();
            formData.append('file', file);

            const response = await fetch('/upload', {
                method: 'POST',
                body: formData
            });

            if (!response.ok) {
                throw new Error(`Upload failed: ${response.status} ${response.statusText}`);
            }

            const contentType = response.headers.get("content-type");
            if (!contentType || !contentType.includes("application/json")) {
                throw new Error("Invalid response format from server");
            }

            const result = await response.json();

            if (result.fileUrl) {
                document.getElementById('fileUrl').value = result.fileUrl;
                progressBar.style.width = '100%';
                publishButton.disabled = false;
                showAlert('success', 3000, 'File uploaded successfully!');
            } else {
                throw new Error(result.error || 'Upload failed: No file URL received');
            }
        } catch (error) {
            console.error('Error during file upload:', error);
            showAlert('error', 5000, `Upload failed: ${error.message}`);
            preview.style.display = 'none';
            publishButton.disabled = false;
            fileInput.value = '';
        }
    }
}

function formatFileSize(bytes) {
    if (bytes === 0) return '0 Bytes';
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

async function publishMessage() {
    const publishButton = document.getElementById('publishButton');
    publishButton.disabled = true;
    
    const data = {
        subject: document.getElementById('subject').value,
        recipient: document.getElementById('recipient').value,
        fileUrl: document.getElementById('fileUrl').value
    };

    if (!data.subject || !data.recipient || !data.fileUrl) {
        showAlert('error', 3000, 'Please fill in all fields and upload an Excel file');
        publishButton.disabled = false;
        return;
    }

    try {
        const response = await fetch('/publish', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        });

        if (!response.ok) {
            throw new Error(`Publish failed: ${response.status} ${response.statusText}`);
        }

        const result = await response.json();

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
    } catch (error) {
        console.error('Error during publish:', error);
        showAlert('error', 5000, `Publish failed: ${error.message}`);
    } finally {
        publishButton.disabled = false;
    }
}