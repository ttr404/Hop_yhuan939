"use strict";

window.onload = (() => {
    const form = document.querySelector('.form');
    const suggestion = document.querySelector('.suggestion');
    const trending = (() => {
        fetch('/suggestion')
            .then(response => response.text()).then(data => {
                const parser = new DOMParser();
                const xml = parser.parseFromString(data, 'text/xml');
                const items = xml.querySelectorAll('item');
                suggestion.innerHTML = '';
                for (let i = 0; i < 3; i++) {
                    const option = document.createElement('li');
                    option.innerText = items[i].querySelector('title').innerHTML;
                    suggestion.appendChild(option);
                }
            });
    });
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

    trending();
})