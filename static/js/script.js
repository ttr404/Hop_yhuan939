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
                });
        };
        const observer = new MutationObserver(mutations => {
            mutations.forEach(mutation => {
                const value = mutation.target.getAttribute('data-replicated-value');
                if (value.length > 0) {
                    fetch('/autocomplete/' + value)
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
        if (form) {
            observer.observe(form, {
                attributes: true,
                attributeFilter: ['data-replicated-value']
            });
        }

        // handle search
        if (search) {
            search.onclick = () => {
                pjax("search?q=" + query.value);
            }
        }

        trending();
    };
}

window.onload = () => {
    let s = window.location, a = window.document, r = a.currentScript;
    if (window.location.pathname === "/") {
        init();
    }
};