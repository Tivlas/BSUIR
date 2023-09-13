from django.shortcuts import render, redirect, get_object_or_404
from .models import Trip, Country
from django.http import HttpResponseNotFound
from django.core.exceptions import PermissionDenied
from .forms import TripForm
from cart.forms import AddTripForm
import requests
from .api_urls import API_URLS


def list_trips(request, trip_country_name=None):
    trips = Trip.objects.all()
    country = None
    sort = request.GET.get('sort')
    min_cost = request.GET.get('min_cost')
    max_cost = request.GET.get('max_cost')
    min_stars = request.GET.get('min_stars')
    max_stars = request.GET.get('max_stars')

    if trip_country_name:
        country = get_object_or_404(Country, name=trip_country_name)
        trips = trips.filter(country=country)

    if min_cost and max_cost:
        trips = trips.filter(cost__gte=min_cost,
                             cost__lte=max_cost)

    if min_stars and max_stars:
        trips = trips.filter(chosen_hotel__stars__gte=min_stars,
                             chosen_hotel__stars__lte=max_stars)

    if sort == 'ascending':
        trips = trips.order_by('cost')
    elif sort == 'descending':
        trips = trips.order_by('-cost')

    countries = Country.objects.all()

    response = requests.get(API_URLS['fact_about_cat'])
    fact_about_cat = None
    if (response.status_code == 200 and request.user.is_authenticated):
        data = response.json()
        fact_about_cat = data['fact']

    return render(request, 'trips/list_trips.html', {'trips': trips, 'countries': countries, 'country': country, 'fact_about_cat': fact_about_cat})


def trip_details(request, id):
    trip = get_object_or_404(Trip, id=id)
    return render(request, 'trips/trip_details.html', {'trip': trip, 'add_to_cart_form': AddTripForm()})


def edit_trip(request, id):
    if not request.user.is_staff:
        raise PermissionDenied("Permission denied!")
    
    trip = None
    try:
        trip = get_object_or_404(Trip, id=id)
    except:
        return HttpResponseNotFound("<h2>No such trip :(</h2>")

    if request.method == 'POST':
        form = TripForm(request.POST, instance=trip)
        if form.is_valid():
            form.save()
            return redirect('/')
    else:
        form = TripForm(instance=trip)

    return render(request, 'trips/edit_trip.html', {'form': form, 'trip': trip})


def delete_trip(request, id):
    if not request.user.is_staff:
        raise PermissionDenied("Permission denied!")

    try:
        trip = get_object_or_404(Trip, id=id)
        trip.delete()
        return redirect("/")
    except:
        return HttpResponseNotFound("<h2>No such trip :(</h2>")
    
def create_trip(request):
    if not request.user.is_staff:
        raise PermissionDenied("Permission denied!")
    
    if request.method == 'POST':
        form = TripForm(request.POST, request.FILES)
        if form.is_valid():
            trip = form.save()
            return redirect('travel:list_trips')
    else:
        form = TripForm()
    
    return render(request, 'trips/create_trip.html', {'form': form})

