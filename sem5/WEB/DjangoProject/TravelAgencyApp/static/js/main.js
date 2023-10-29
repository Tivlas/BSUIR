const banners = document.querySelectorAll('.circular-image');
const rotationInterval = 3000;
let currentBannerIndex = 0;
let rotationTimeout;


function rotateBanners() {
    banners[currentBannerIndex].style.display = 'none';
    currentBannerIndex = (currentBannerIndex + 1) % banners.length;
    banners[currentBannerIndex].style.display = 'inline-block';
    rotationTimeout = setTimeout(rotateBanners, rotationInterval);
}

rotateBanners();

function checkPageFocus() {
    if (document.hidden) {

        clearTimeout(rotationTimeout);
    } else {

        rotateBanners();
    }
}

document.addEventListener('visibilitychange', checkPageFocus);