document.addEventListener('DOMContentLoaded', function () {
    var uploadForm = document.querySelector('form[action="/uploadImage"]');

    uploadForm.addEventListener('submit', function (event) {
        event.preventDefault(); // Prevent the default form submission

        var formData = new FormData(uploadForm);
        var xhr = new XMLHttpRequest();

        xhr.open('POST', '/uploadImage', true);

        xhr.onload = function () {
            if (this.status === 200) {
                // If the request was successful, display the success message
                var successMessage = document.createElement('p');
                successMessage.textContent = 'Upload successful!';
                uploadForm.appendChild(successMessage);
            } else {
                // If the request failed, display an error message
                var errorMessage = document.createElement('p');
                errorMessage.textContent = 'Upload failed. Please try again.';
                uploadForm.appendChild(errorMessage);
            }
        };

        xhr.send(formData);
    });
});
