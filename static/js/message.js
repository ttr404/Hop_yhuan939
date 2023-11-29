document.addEventListener('DOMContentLoaded', function () {
    var uploadForm = document.querySelector('form[action="/uploadImage"]');

    uploadForm.addEventListener('submit', function (event) {
        event.preventDefault(); // Prevent the default form submission

        var formData = new FormData(uploadForm);

        fetch('/uploadImage', {
            method: 'POST',
            body: formData
        })
        .then(response => {
            if (response.ok) {
                return response.text();
            } else {
                throw new Error('Network response was not ok.');
            }
        })
        .then(text => {
            var successMessage = document.createElement('p');
            successMessage.textContent = 'Upload successful!';
            uploadForm.appendChild(successMessage);
        })
        .catch(error => {
            var errorMessage = document.createElement('p');
            errorMessage.textContent = 'Upload failed. Please try again.';
            uploadForm.appendChild(errorMessage);
            console.error('There has been a problem with your fetch operation:', error);
        });
    });
});
