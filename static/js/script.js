"use strict";
window.onload = (() => {
    let timeout;
    const form = document.querySelector('.form');
    const suggestion = document.querySelector('.suggestion');
    const observer = new MutationObserver(mutations => {
        console.log(mutations);
        mutations.forEach(mutation => {
                const value = mutation.target.getAttribute('data-replicated-value');
                if (value.length > 0) {
                    fetch('https://liberty.akamaized.net/suggestion/' + value, {
                        mode: 'cors'
                    }).then(response => response.json()).then(data => {
                        suggestion.innerHTML = '';
                        for (let i = 0; i < 5; i++) {
                            const option = document.createElement('li');
                            option.innerHTML = data[1][i];
                            suggestion.appendChild(option);
                        }
                    }
                    );
                } else {
                    suggestion.innerHTML = '';
                }
            }
        );
    });

    // observe form attribute "data-replicated-value"
    observer.observe(form, {
        attributes: true,
        attributeFilter: ['data-replicated-value']
    });
})