from django.test import TestCase, Client
from django.urls import reverse
from django.contrib.auth import get_user_model
from travel.models import Trip, Country, Hotel
from order.models import Order, OrderItem
from cart.cart import Cart


class OrderViewsTestCase(TestCase):
    def setUp(self):
        self.user = get_user_model().objects.create_user(email="test@gmail.com",
            username='testuser', password='testpassword',first_name="a",last_name='b',date_of_birth='2003-04-12',phone_number='+375 (44) 123-45-67'
        )
        

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


    def test_list_orders_view(self):
        client = Client()
        client.force_login(self.user)
        order1 = Order.objects.create(client=self.user)
        OrderItem.objects.create(order=order1, trip=self.trip, cost=self.trip.cost, quantity=2)

        order2 = Order.objects.create(client=self.user)
        OrderItem.objects.create(order=order2, trip=self.trip, cost=self.trip.cost, quantity=1)

        self.user.is_staff = True
        self.user.save()

        response = client.get(reverse('order:list_orders'))
        self.assertEqual(response.status_code, 200)

        orders = response.context['orders']
        self.assertEqual(orders.count(), 2)

        self.assertContains(response, order1.client.username)
        self.assertContains(response, order2.client.username)
        self.assertContains(response, self.trip.name)

    def test_create_order_view_permission_denied(self):
        client = Client()
        client.force_login(self.user)
        client.logout()

        response = client.post(reverse('order:create_order'))
        self.assertEqual(response.status_code, 403)

    def test_list_orders_view_permission_denied(self):
        client = Client()
        client.force_login(self.user)
        client.logout()

        response = client.get(reverse('order:list_orders'))
        self.assertEqual(response.status_code, 403)
