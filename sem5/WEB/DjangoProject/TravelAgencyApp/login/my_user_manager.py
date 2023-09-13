from django.contrib.auth.models import BaseUserManager


class MyUserManager(BaseUserManager):
    def create_user(self, email, first_name, last_name, date_of_birth, phone_number, password=None, **extra_fields):
        if not email:
            raise ValueError('The given email must be set')

        email = self.normalize_email(email)
        user = self.model(email=email, first_name=first_name, last_name=last_name,
                          date_of_birth=date_of_birth, phone_number=phone_number, **extra_fields)
        user.set_password(password)
        user.save(using=self._db)
        return user

    def create_superuser(self, email='', first_name='', last_name='', date_of_birth='2000-10-10', phone_number='+375 (29) 123-45-67', password=None, **extra_fields):
        extra_fields.setdefault('is_staff', True)
        extra_fields.setdefault('is_superuser', True)

        if extra_fields.get('is_staff') is not True:
            raise ValueError('Superuser is_staff must be True.')
        if extra_fields.get('is_superuser') is not True:
            raise ValueError('Superuser is_superuser must be True.')

        return self.create_user(email, first_name, last_name, date_of_birth, phone_number, password, **extra_fields)
