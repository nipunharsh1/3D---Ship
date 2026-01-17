let currentSlide = 0;
const slides = document.querySelectorAll('.slide');
const totalSlides = slides.length;
const demoOverlay = document.getElementById('demo-script');

function showSlide(index) {
    slides.forEach(slide => slide.classList.remove('active'));
    slides[index].classList.add('active');
}

function nextSlide() {
    if (currentSlide < totalSlides - 1) {
        currentSlide++;
        showSlide(currentSlide);
    }
}

function prevSlide() {
    if (currentSlide > 0) {
        currentSlide--;
        showSlide(currentSlide);
    }
}

function toggleDemo() {
    demoOverlay.classList.toggle('hidden');
}

// Keyboard Navigation
document.addEventListener('keydown', (e) => {
    switch(e.key) {
        case 'ArrowRight':
        case 'ArrowDown':
        case ' ':
            nextSlide();
            break;
        case 'ArrowLeft':
        case 'ArrowUp':
            prevSlide();
            break;
        case 's':
        case 'S':
            toggleDemo();
            break;
        case 'Escape':
            if (!demoOverlay.classList.contains('hidden')) {
                toggleDemo();
            }
            break;
    }
});
