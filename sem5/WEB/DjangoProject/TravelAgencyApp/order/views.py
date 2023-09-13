from django.shortcuts import render
from .models import OrderItem
from cart.cart import Cart
from .models import Order
from django.core.exceptions import PermissionDenied
import requests
from .api_urls import API_URLS


def create_order(request):
    if not request.user.is_authenticated:
        raise PermissionDenied("Permission denied. Sign in first.")

    cart = Cart(request)
    cart_is_empty = len(cart) == 0
    if not cart_is_empty and request.method == 'POST':
        order = Order.objects.create(client=request.user)

        for item in cart:
            OrderItem.objects.create(order=order,
                                     trip=item['trip'],
                                     cost=item['cost'],
                                     quantity=item['quantity'])
        cart.clear()
        response = requests.get(API_URLS['random_dog_picture'])
        image_url = None
        if response.status_code == 200:
            data = response.json()
            image_url = data['message']
        return render(request, 'order_created.html', {'image_url': image_url})

    return render(request, 'create_order.html',
                  {'cart': cart, 'empty': cart_is_empty})


def list_orders(request):
    if not request.user.is_staff:
        raise PermissionDenied("Permission denied.")
    orders = Order.objects.all().order_by('client')
    return render(request, 'list_orders.html', {'orders': orders})
