"use strict";

let initHome, initSearch, loadHistory, handleQuery, swiper, onSwiper;

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

if (typeof swiper === "undefined") {
    onSwiper = () => {
        const swiper = new Swiper(".swiper", {
            mousewheel: true,
            slidesPerView: "auto",
            scrollbar: {
                el: ".swiper-scrollbar",
            }
        });
    }
    swiper = () => {
        const swiperScript = document.createElement('script');
        swiperScript.src = "https://cdn.jsdelivr.net/npm/swiper@11/swiper-bundle";
        document.body.appendChild(swiperScript);
        swiperScript.onload = () => {
            onSwiper();
        };
    };
} else {
    onSwiper();
}

if (typeof initSearch === "undefined") {
    initSearch = () => {
        let url = new URL(window.location);
        const urlParams = new URLSearchParams(url.search);
        const collapse = document.querySelector('aside button');
        if (typeof Swiper === "undefined") {
            const swiperScript = document.createElement('script');
            swiperScript.src = "https://cdn.jsdelivr.net/npm/swiper@11/swiper-bundle.min.js";
            document.body.appendChild(swiperScript);
            swiperScript.onload = () => {
                const swiper = new Swiper(".swiper", {
                    mousewheel: true,
                    slidesPerView: "auto",
                    scrollbar: {
                        el: ".swiper-scrollbar",
                    }
                });
            };
        } else {
            const swiper = new Swiper(".swiper", {
                mousewheel: true,
                slidesPerView: "auto",
                scrollbar: {
                    el: ".swiper-scrollbar",
                }
            });
        }

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

if (typeof handleQuery === "undefined") {
    handleQuery = () => {
        const template = document.querySelector('template.card');
        const container = document.querySelector('.swiper-wrapper');
        let url = new URL(window.location);
        const urlParams = new URLSearchParams(url.search);

        const onSearch = ({ data }) => {
            const clone = template.content.cloneNode(true);
            const box = clone.querySelector('.card');
            const img = clone.querySelector('img');
            const title = clone.querySelector('span');
            const content = clone.querySelector('p');
            const text = data.name.charAt(0).toUpperCase() + data.name.slice(1);
            title.innerHTML = text;
            content.innerText = data.summary;
            img.src = data.url;
            // box.onclick = () => {
            //     pjax();
            // }
            container.appendChild(clone);
        }

        urlParams.set('type', 'json');
        url.search = urlParams;
        fetch(url)
            .then(response => response.json())
            .then(data => {
                for (let i = 0; i < data.result.length; i++) {
                    onSearch({ data: data.result[i] });
                }
            });
    };
}

if (typeof loadHistory === "undefined") {
    loadHistory = () => {
        const template = document.querySelector('template.history');
        const container = document.querySelector('aside ul');
        let url = new URL(window.location);
        const urlParams = new URLSearchParams(url.search);

        const onCreateHistory = ({ data }) => {
            const clone = template.content.cloneNode(true);
            const box = clone.querySelector('li');
            const span = clone.querySelector('span');
            const text = data.charAt(0).toUpperCase() + data.slice(1);
            span.innerText = text;
            span.onclick = (event) => {
                // don't allow click through
                event.stopPropagation();
                pjax("search?q=" + data);
            };
            box.onclick = () => {
                box.parentNode.removeChild(box);
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
}

window.onload = () => {
    let s = window.location, a = window.document, r = a.currentScript;
    if (s.pathname === "/") {
        initHome();
    } else if (s.pathname.startsWith("/search")) {
        initSearch();
        handleQuery();
        loadHistory();
    } else if (s.pathname.startsWith("/admin")) {
        if (typeof dash === "undefined") {
            const dashScript = document.createElement('script');
            dashScript.src = "/static/js/message.js";
            document.body.appendChild(dashScript);
            dashScript.onload = () => {
                dash();
            };
        } else {
            dash();
        }
    }
};