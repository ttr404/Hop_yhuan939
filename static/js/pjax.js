const pjax = (url) => {
    fetch(url)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.text();
        })
        .then(html => {
            const parser = new DOMParser();
            const doc = parser.parseFromString(html, 'text/html');

            // Update the head - specifically look for changes in stylesheets
            const newHead = doc.head;
            const oldHead = document.head;

            // Remove any existing stylesheets
            Array.from(oldHead.querySelectorAll('link[rel="stylesheet"], style')).forEach(el => el.remove());

            // Append new stylesheets from the new page
            Array.from(newHead.querySelectorAll('link[rel="stylesheet"], style')).forEach(newStyle => {
                oldHead.appendChild(newStyle);
            });

            // // Replace the entire body's content
            // document.body.innerHTML = doc.body.innerHTML;

            // Replace the entire body's content
            const oldBody = document.body;
            const newBody = doc.body;
            oldBody.innerHTML = newBody.innerHTML;

            // Check if the pjax.js script has already been loaded
            if (typeof window.pjax === 'undefined') {
                // Append scripts from the new page
                Array.from(newBody.querySelectorAll('script')).forEach(newScript => {
                    const script = document.createElement('script');
                    script.textContent = newScript.textContent;
                    script.src = newScript.src;
                    script.async = false; // Scripts are loaded synchronously by default
                    oldBody.appendChild(script);
                });

                // Append pjax.js script to the body
                const pjaxScript = document.createElement('script');
                pjaxScript.src = '/static/js/pjax.js';
                pjaxScript.async = true;
                document.body.appendChild(pjaxScript);
            }

            // Update the browser's URL and history
            history.pushState({}, '', url);

            // Update the title of the page
            document.title = doc.title;
        })
        .catch(error => {
            console.error('Failed to load page: ', error);
        });

    // Avoid duplicated declaration of pjax
    window.pjax = true;
}
window.onpopstate = () => {
    pjax(location.pathname);
}