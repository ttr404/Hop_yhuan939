let dash;
// document.addEventListener('DOMContentLoaded', function () {
if (typeof dash === "undefined") {
    dash = () => {
        // var uploadForm = document.querySelector('form[action="/uploadImage"]');
        const subBtn = document.querySelector('.btn.round');
        const urlInput = document.querySelector('[name="URL"]');
        const uploadForm = document.querySelector('.urlForm')

        subBtn.onclick = () => {
            // event.preventDefault(); // Prevent the default form submission

            // var formData = new FormData(uploadForm);
            const data = new FormData();
            data.append("URL", urlInput.value);

            fetch('/uploadImage', {
                method: 'POST',
                body: data
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
        };
    };
}
