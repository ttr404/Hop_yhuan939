"use strict";

let initHome, initSearch, loadHistory;

if (typeof initHome === "undefined") {
    initHome = () => {
        const form = document.querySelector('.form');
        const query = document.querySelector('.form textarea');
        const suggestion = document.querySelector('.suggestion');
        const search = document.querySelector('.form button:last-child');

        const trending = () => {
            fetch('/suggestion')
                .then(response => response.text()).then(data => {
                    const parser = new DOMParser();
                    const xml = parser.parseFromString(data, 'text/xml');
                    const items = xml.querySelectorAll('item');
                    suggestion.innerHTML = '';
                    for (let i = 0; i < 3; i++) {
                        const option = document.createElement('li');
                        option.innerText = items[i].querySelector('title').innerHTML;
                        option.onclick = () => {
                            pjax("search?q=" + option.innerText);
                        };
                        suggestion.appendChild(option);
                    }
                }).catch(error => {
                    console.error('Failed to load page: ', error);
                });
        };
        const observer = new MutationObserver(mutations => {
            mutations.forEach(mutation => {
                const value = mutation.target.getAttribute('data-replicated-value').trim();
                if (value.length > 0) {
                    fetch('/suggestion/' + value)
                        .then(response => response.json()).then(data => {
                            suggestion.innerHTML = '';
                            for (let i = 0; i < 5; i++) {
                                const option = document.createElement('li');
                                option.innerText = data[1][i];
                                option.onclick = () => {
                                    pjax("search?q=" + option.innerText);
                                };
                                suggestion.appendChild(option);
                            }
                        }
                        );
                } else {
                    trending();
                }
            }
            );
        });

        // observe form attribute "data-replicated-value"
        observer.observe(form, {
            attributes: true,
            attributeFilter: ['data-replicated-value']
        });

        // handle meta and enter key pressed
        query.onkeydown = (event) => {
            if (event.metaKey || event.ctrlKey) {
                if (event.keyCode === 13) {
                    pjax("search?q=" + query.value);
                }
            }
        }

        // handle search
        search.onclick = () => {
            pjax("search?q=" + query.value);
        };

        trending();
    };
}

if (typeof initSearch === "undefined") {
    initSearch = () => {
        let url = new URL(window.location);
        const urlParams = new URLSearchParams(url.search);
        const collapse = document.querySelector('aside button');
        const swiper = new Swiper(".swiper", {
            mousewheel: true,
            slidesPerView: "auto",
            scrollbar: {
                el: ".swiper-scrollbar",
            }
        });

        collapse.onclick = () => {
            document.querySelector('main').classList.toggle('collapsed');
        };
        urlParams.set('type', 'json');
        url.search = urlParams;
        fetch(url)
            .then(response => response.json())
            .then(json => json.result)
            .then(data => {
                console.log(data);
            });

    };
}





import LocalStorage from "./LocalStorage.js";
if (typeof loadHistory === "undefined") {
    const createTaskForm = document.querySelector('.form');
createTaskForm.addEventListener('recordButton', (e) => {
    e.preventDefault();
    const value = createTaskField.value;
    if (value) {
      const data = {
        value,
        checked: false
      };
      storage.create(data);
      onCreateHistory({data});
      createTaskForm.reset();
    }
  });
    loadHistory = () => {
        //--------------------------------
        
        const storage = new LocalStorage();
        const items = storage.items;
        items.forEach((data) => {
            onCreateHistory({ data });
        });
        //--------------------------------
        const template = document.querySelector('template.history');
        const container = document.querySelector('aside ul');
        let url = new URL(window.location);
        const urlParams = new URLSearchParams(url.search);

        //---------------
        const onCreateHistory = ({ data }) => {
            const clone = template.content.cloneNode(true);
            const box = clone.querySelector('li');
            const span = clone.querySelector('span');
            const text = data.charAt(0).toUpperCase() + data.slice(1);
            span.innerText = data.value;
            span.onclick = (event) => { // textbox inside the history box
                // don't allow click through
                event.stopPropagation();
                pjax("search?q=" + data);
                storage.update({ data });
            };

            box.onclick = () => { // history box, including the delete button
                box.parentNode.removeChild(box);
                storage.delete({ data });
            }
            container.appendChild(clone);
        };

        urlParams.set('h', 'test');
        url.search = urlParams;
        fetch(url)
            .then(response => response.json())
            .then(data => {
                for (let i = 0; i < data.length; i++) {
                    onCreateHistory({ data: data[i] });
                }
            });
    }
    //---------------
}

window.onload = () => {
    let s = window.location, a = window.document, r = a.currentScript;
    if (s.pathname === "/") {
        initHome();
    } else if (s.pathname.startsWith("/search")) {
        initSearch();
        loadHistory();
    }
};