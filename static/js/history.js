import LocalStorage from './LocalStorage.js';

let s = window.location, a = window.document, r = a.currentScript, loadHistory;

if (typeof loadHistory === "undefined") {
    loadHistory = () => {
        const storage = new LocalStorage();
        const localHistory = storage.items;
        const template = document.querySelector('template.history');
        const container = document.querySelector('aside ul');
        const url = new URL(window.location);
        const query = url.searchParams.get('q') || "";

        const onCreateHistory = ({ data }) => {
            const clone = template.content.cloneNode(true);
            const box = clone.querySelector('li');
            const span = clone.querySelector('span');
            const query = data.query;
            const text = query.charAt(0).toUpperCase() + query.slice(1) ?? data;
            span.innerText = text;
            span.onclick = (event) => {
                // don't allow click through
                event.stopPropagation();
                pjax("search?q=" + query);
            };
            box.onclick = () => {
                storage.delete(data);
                box.parentNode.removeChild(box);
            }
            container.appendChild(clone);
        };

        localHistory.forEach((data) => {
            onCreateHistory({ data });
        });

        if (query.length > 0) {
            const data = { query };
            storage.create(data);
            onCreateHistory({ data });
        }
    }
}

// if (s.pathname.startsWith("/search")) {
//     loadHistory();
// }

// copy current window.onload event addd add loadHistory

if (typeof window.onload === "undefined") {
    window.onload = () => {
        loadHistory();
    }
}
else {
    const oldOnload = window.onload;
    window.onload = () => {
        oldOnload();
        if (window.location.pathname.startsWith("/search")) {
            loadHistory();
        }
    }
}