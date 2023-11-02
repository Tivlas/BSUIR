var imageElements = document.getElementsByTagName('img');
var lastScrollPosition = document.documentElement.scrollTop;
var degrees = 1;

function isScrollingDown() {
    var scroll = document.documentElement.scrollTop;
    var scrollingDown = scroll > lastScrollPosition;
    lastScrollPosition = scroll;
    return scrollingDown;
}

function rotateImagesOnScroll() {
    isDown = isScrollingDown();
    for (let image of imageElements) {
        degrees += isDown ? 1 : -1;
        image.style.transform = 'rotate(' + degrees + 'deg)';
    }
}

window.addEventListener('scroll', rotateImagesOnScroll);

// Обратный отсчет
var lastDisplayedTime;
function displayCountdown(lastDisplayedTime) {
    const countdownElement = document.getElementById('countdown');

    function updateCountdown() {
        const currentTime = new Date().getTime();
        const timeLeft = lastDisplayedTime - currentTime;

        if (timeLeft <= 0) {
            countdownElement.textContent = 'Обратный отсчет завершен';
        } else {
            const minutes = Math.floor((timeLeft % 3600000) / 60000);
            const seconds = Math.floor((timeLeft % 60000) / 1000);
            countdownElement.textContent = `${minutes}:${seconds}`;
        }
    }

    updateCountdown();
    const intervalId = setInterval(updateCountdown, 1000);

    localStorage.setItem('lastDisplayedTime', lastDisplayedTime);

    window.onbeforeunload = () => {
        clearInterval(intervalId);
    };
}

lastDisplayedTime = localStorage.getItem('lastDisplayedTime');

if (lastDisplayedTime) {
    displayCountdown(Number(lastDisplayedTime));
} else {
    lastDisplayedTime = new Date().getTime() + 3600000;
    displayCountdown(lastDisplayedTime);
}