from django.shortcuts import render, redirect, get_object_or_404
from django.views.decorators.http import require_POST
from travel.models import Trip
from .cart import Cart
from .forms import AddTripForm


@require_POST
def cart_add(request, trip_id):
    cart = Cart(request)
    trip = get_object_or_404(Trip, id=trip_id)
    form = AddTripForm(request.POST)
    if form.is_valid():
        form_data = form.cleaned_data
        cart.add(trip=trip,
                 quantity=form_data['quantity'],
                 update_quantity=form_data['update'])
    return redirect('cart:cart_details')


def cart_remove(request, trip_id):
    cart = Cart(request)
    trip = get_object_or_404(Trip, id=trip_id)
    cart.remove(trip)
    return redirect('cart:cart_details')


def cart_details(request):
    cart = Cart(request)
    return render(request, 'cart_details.html', {'cart': cart})
