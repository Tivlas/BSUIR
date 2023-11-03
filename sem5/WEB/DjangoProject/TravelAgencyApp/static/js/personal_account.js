
var birthDate = NaN;
while (isNaN(birthDate) || birthDate > today) {
    var prom = prompt("Enter tour birth date:");
    birthDate = new Date(prom);
}
if (prom !== null) {
    var age = today.getFullYear() - birthDate.getFullYear();
    if (age >= 18) {
        var weekdays = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
        var dayOfWeek = weekdays[birthDate.getDay()];
        alert("You are an adult. The day of the week for your birth date is: " + dayOfWeek);
    } else {
        alert("You are a minor. Parental consent is required to use this website.");
    }
}



var books = {
    'Smith': [{ title: 'Book 1', year: 1958 }, { title: 'Book 4', year: 1970 }],
    'Johnson': [{ title: 'Book 2', year: 1965 }, { title: 'Book 5', year: 1959 }],
    'Williams': [{ title: 'Book 3', year: 1962 }, { title: 'Book 6', year: 1963 }]
};

function searchBooks(event) {
    event.preventDefault();

    var authorInput = document.getElementById('authorInput');
    var author = authorInput.value;

    var results = document.getElementById('results');
    results.innerHTML = '';

    var foundBooks = books[author];


    if (foundBooks && foundBooks.length > 0) {
        var resultList = document.createElement('ul');
        for (var book of foundBooks) {
            if (book.year >= 1960) {
                var listItem = document.createElement('li');
                listItem.textContent = book.title;
                resultList.appendChild(listItem);
            }
        }
        results.appendChild(resultList);
    } else {
        results.textContent = 'No books found by this author.';
    }
}


var searchForm = document.getElementById('searchForm');
searchForm.addEventListener('submit', searchBooks);


// Классы пункт 10

function withDiscount(getPriceFunc, discount) {
    return function () {
        return getPriceFunc.call(this) * (1 - discount / 100);
    }
}

function Trip(destination, duration, price) {
    this.destination = destination;
    this.duration = duration;
    this.price = price;
}

Trip.prototype.getDestination = function () {
    return this.destination;
};

Trip.prototype.setDestination = function (destination) {
    this.destination = destination;
};

Trip.prototype.getDuration = function () {
    return this.duration;
};

Trip.prototype.setDuration = function (duration) {
    this.duration = duration;
};

Trip.prototype.getPrice = withDiscount(function () {
    return this.price;
}, 20);

Trip.prototype.setPrice = function (price) {
    this.price = price;
};

function AllInclusiveTrip(destination, duration, price, hotel) {
    Trip.call(this, destination, duration, price);
    this.hotel = hotel;
}

AllInclusiveTrip.prototype = Object.create(Trip.prototype);
AllInclusiveTrip.prototype.constructor = AllInclusiveTrip;

AllInclusiveTrip.prototype.getHotel = function () {
    return this.hotel;
};

AllInclusiveTrip.prototype.setHotel = function (hotel) {
    this.hotel = hotel;
};

///////////////////////////////
class Trip2 {
    constructor(destination, duration, price) {
        this.destination = destination;
        this.duration = duration;
        this.price = price;
    }

    get getDestination() {
        return this.destination;
    }

    set setDestination(destination) {
        this.destination = destination;
    }

    get getDuration() {
        return this.duration;
    }

    set setDuration(duration) {
        this.duration = duration;
    }

    get getPrice() {
        return this.price;
    }

    set setPrice(price) {
        this.price = price;
    }
}

class AllInclusiveTrip2 extends Trip2 {
    constructor(destination, duration, price, hotel) {
        super(destination, duration, price);
        this.hotel = hotel;
    }

    get getHotel() {
        return this.hotel;
    }

    set setHotel(hotel) {
        this.hotel = hotel;
    }
}