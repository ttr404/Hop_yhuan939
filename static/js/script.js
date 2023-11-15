"use strict";

let init;

if (typeof init === "undefined") {
    init = () => {
        const form = document.querySelector('.form');
        const query = document.querySelector('.form textarea');
        const suggestion = document.querySelector('.suggestion');
        const search = document.querySelector('.form button');
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
            if(event.metaKey || event.ctrlKey) {
                if (event.keyCode === 13) {
                    pjax("search?q=" + query.value);
                }
            }
        }

        // handle search
        search.onclick = () => {
            pjax("search?q=" + query.value);
        }

        trending();
    };
}

window.onload = () => {
    let s = window.location, a = window.document, r = a.currentScript;
    if (s.pathname === "/") {
        init();
    }
};