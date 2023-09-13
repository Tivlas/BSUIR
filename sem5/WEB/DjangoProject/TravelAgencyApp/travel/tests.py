from django.test import TestCase, Client
from django.urls import reverse
from login.models import MyUser
from .models import Trip, Country, Hotel
from contextlib import contextmanager
from datetime import date


class TravelAppTests(TestCase):
    @contextmanager
    def modify_user_is_staff(self, user, is_staff):
        original_is_staff = user.is_staff
        user.is_staff = is_staff
        user.save()
        yield
        user.is_staff = original_is_staff
        user.save()

    def setUp(self):
        self.client = Client()
        self.user = MyUser.objects.create_user(email="test@gmail.com",
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

    def test_list_trips(self):
        url = reverse('travel:list_trips')
        response = self.client.get(url)
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'trips/list_trips.html')
        self.assertIn(self.trip, response.context['trips'])

    def test_trip_details(self):
        url = reverse('travel:trip_details', args=[self.trip.id])
        response = self.client.get(url)
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'trips/trip_details.html')
        self.assertEqual(response.context['trip'], self.trip)

    def test_edit_trip(self):
        url = reverse('travel:edit_trip', args=[self.trip.id])
        with self.modify_user_is_staff(self.user,is_staff=True):
            self.client.force_login(self.user)
            self.client
            response = self.client.get(url)
            self.assertEqual(response.status_code, 200)
            self.assertTemplateUsed(response, 'trips/edit_trip.html')
            self.assertEqual(response.context['trip'], self.trip)

    def test_delete_trip(self):
        url = reverse('travel:delete_trip', args=[self.trip.id])
        with self.modify_user_is_staff(self.user,is_staff=True):
            self.client.force_login(self.user)
            response = self.client.post(url)
            self.assertEqual(response.status_code, 302)
            self.assertEqual(response.url, '/')
            self.assertFalse(Trip.objects.filter(id=self.trip.id).exists())

    def test_create_trip(self):
        url = reverse('travel:create_trip')
        with self.modify_user_is_staff(self.user,is_staff=True):
            self.client.force_login(self.user)
            response = self.client.get(url)
            self.assertEqual(response.status_code, 200)
            self.assertTemplateUsed(response, 'trips/create_trip.html')

            data = {
                'name': 'New Trip',
                'country': self.country.id,
                'cost': 200,
                'chosen_hotel': self.hotel.id,
                'duration': 2,
                'departure_date': date.today()
            }
            response = self.client.post(url, data)

        self.assertEqual(response.status_code, 302)
        self.assertTrue(Trip.objects.filter(name='New Trip').exists())

    def test_list_trips_by_country(self):
        url = reverse('travel:list_trips_by_country', args=[self.country.name])
        response = self.client.get(url)
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'trips/list_trips.html')
        self.assertEqual(response.context['country'], self.country)
        self.assertIn(self.trip, response.context['trips'])
