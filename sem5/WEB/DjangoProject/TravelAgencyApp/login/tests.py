from django.test import TestCase, Client
from django.urls import reverse
from django.contrib.auth import get_user_model


class MyUserTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.signup_url = reverse('login:signup')
        self.signin_url = reverse('login:signin')
        self.logout_url = reverse('login:logout')
        self.user_data = {
            'username': "NAME",
            'email': 'test@example.com',
            'first_name': 'ivan',
            'last_name': 'ivanov',
            'date_of_birth': '2000-01-01',
            'phone_number': '+375 (44) 123-34-45',
            'password1': 'qazwsx!123',
            'password2': 'qazwsx!123',
        }

    def test_signup_view(self):
        response = self.client.get(self.signup_url)
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'signup.html')

        response = self.client.post(self.signup_url, self.user_data)
        self.assertEqual(response.status_code, 302)
        self.assertRedirects(response, self.signin_url)

        User = get_user_model()
        self.assertTrue(User.objects.filter(username='NAME').exists())

    def test_signin_view(self):
        User = get_user_model()
        user = User.objects.create_user(email='test@example.com', first_name='ivan', last_name='ivanov',
                                        date_of_birth='2000-01-01', phone_number='1234567890',
                                        password='qazwsx!123')
        user.username = "NAME"
        user.save()
        response = self.client.get(self.signin_url)
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'signin.html')

        data = {
            'username': 'NAME',
            'password': 'qazwsx!123'
        }
        response = self.client.post(self.signin_url, data)
        self.assertEqual(response.status_code, 302)
        self.assertRedirects(response, '/')

        self.assertTrue(response.wsgi_request.user.is_authenticated)

    def test_logout_view(self):
        User = get_user_model()
        user = User.objects.create_user(email='test@example.com', first_name='ivan', last_name='ivanov',
                                        date_of_birth='2000-01-01', phone_number='1234567890',
                                        password='qazwsx!123')

        self.client.login(email='test@example.com', password='qazwsx!123')

        response = self.client.get(self.logout_url)
        self.assertEqual(response.status_code, 302)
        self.assertRedirects(response, '/')

        self.assertFalse(self.client.session.get('_auth_user_id', False))
