let initHome, initSearch, handleQuery, onHistory, swiper, onSwiper, tts;

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
                            for (let i = 0; i < 3; i++) {
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

if (typeof onHistory === "undefined") {
    onHistory = () => {
        const storageScript = document.createElement('script');
        const historyScript = document.createElement('script');
        storageScript.src = "../static/js/LocalStorage.js";
        historyScript.src = "../static/js/history.js";
        storageScript.type = "module";
        historyScript.type = "module";
        document.body.appendChild(storageScript);
        document.body.appendChild(historyScript);
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
            const box = clone.querySelector('.swiper-slide');
            const img = clone.querySelector('img');
            const title = clone.querySelector('span');
            const content = clone.querySelector('p');
            const text = data.name.charAt(0).toUpperCase() + data.name.slice(1);
            title.innerText = text;
            content.innerHTML = data.summary;
            img.src = data.url;
            box.onclick = () => {
                pjax("search?q=" + data.name);
            }
            container.appendChild(clone);
        }

        urlParams.set('type', 'json');
        url.search = urlParams;
        fetch(url)
            .then(response => response.json())
            .then(data => {
                if(data.result == "No result found.") {
                    // throw error
                    throw new Error(urlParams.get('q'));
                }
                for (let i = 0; i < data.result.length; i++) {
                    onSearch({ data: data.result[i] });
                }
            }).catch(error => {
                onSearch({ data: { name: "No results found", summary: "<center>Try searching for something else</center>", url: "https://cdn.discordapp.com/attachments/1101017132482957363/1179244005259751544/365cent_no_found_error_image_modern_simple_52184e04-26ad-471b-b93a-231d91162668.png?ex=65791390&is=65669e90&hm=8e740d9db4cb94726c03be43af18c5726b8ce5085525dc73b69beefef452fb57&" } });
                console.error('Failed to load result: ', error);
            });
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

window.onload = () => {
    let s = window.location, a = window.document, r = a.currentScript;
    if (s.pathname === "/") {
        initHome();
    } else if (s.pathname.startsWith("/search")) {
        initSearch();
        initHome();
        handleQuery();
        onHistory();
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