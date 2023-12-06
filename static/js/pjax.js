const pjax = (url) => {
    // Store the current URL as the previous page
    localStorage.setItem('previousPage', location.href);

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
            // Array.from(oldHead.querySelectorAll('link[rel="stylesheet"], style')).forEach(el => el.remove());

            // Append new stylesheets from the new page
            // Array.from(newHead.querySelectorAll('link[rel="stylesheet"], style')).forEach(newStyle => {
            //     oldHead.appendChild(newStyle);
            // });
            
            const newStyles = Array.from(newHead.querySelectorAll('link[rel="stylesheet"], style'));
            const oldStyles = Array.from(oldHead.querySelectorAll('link[rel="stylesheet"], style'));

            oldStyles.forEach(style => {
                if (!newStyles.includes(style)) {
                    style.remove();
                }
            });

            newStyles.forEach(style => {
                if (!oldStyles.includes(style)) {
                    oldHead.appendChild(style);
                }
            });

            document.body.innerHTML = doc.body.innerHTML;
            history.pushState({}, '', url);
            document.title = doc.title;
            localStorage.setItem('nextPage', url);

            // Emit the onload event
            window.dispatchEvent(new Event('load'));
        })
        .catch(error => {
            console.error('Failed to load page: ', error);
        });
};

window.onpopstate = () => {
    pjax(location.pathname);
};
