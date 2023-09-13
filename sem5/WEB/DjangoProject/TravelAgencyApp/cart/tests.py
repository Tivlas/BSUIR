from django.test import TestCase, Client
from django.urls import reverse
from travel.models import Trip, Country, Hotel
from django.conf import settings
from .cart import Cart


class CartViewsTestCase(TestCase):
    def setUp(self):
        self.country = Country.objects.create(name='Test Country')
        self.hotel = Hotel()
        self.hotel.name = 'hotel'
        self.hotel.stars = 4
        self.hotel.price_per_day = 222
        self.hotel.country = self.country
        self.hotel.save()
        self.trip = Trip.objects.create(
            name='Test Trip',
            country=self.country,
            cost=100,
            chosen_hotel=self.hotel
        )
        self.cart_add_url = reverse('cart:cart_add', args=[self.trip.id])
        self.cart_remove_url = reverse('cart:cart_remove', args=[self.trip.id])
        self.cart_details_url = reverse('cart:cart_details')

    def test_cart_add_view(self):
        client = Client()
        response = client.post(self.cart_add_url, {'quantity': 2, 'update': False})
        self.assertEqual(response.status_code, 302) 

        cart = Cart(client)
        self.assertTrue(str(self.trip.id) in cart.cart)
        self.assertEqual(cart.cart[str(self.trip.id)]['quantity'], 2)

    def test_cart_remove_view(self):
        client = Client()
        cart = Cart(client)
        client.session[settings.CART_SESSION_ID] = cart.cart
        cart.add(trip=self.trip, quantity=2, update_quantity=False)

        response = client.post(self.cart_remove_url)
        self.assertEqual(response.status_code, 302)

        cart = Cart(client)
        self.assertFalse(str(self.trip.id) in cart.cart)
