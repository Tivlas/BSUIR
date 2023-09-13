from django.db import models
from django.contrib.auth.models import AbstractUser
from .my_user_manager import MyUserManager


class MyUser(AbstractUser):
    first_name = models.CharField(max_length=200)
    last_name = models.CharField(max_length=200)
    date_of_birth = models.DateField()
    email = models.EmailField()
    phone_number = models.CharField(max_length=50)
    REQUIRED_FIELDS = ['first_name', 'last_name',
                       'email', 'date_of_birth', 'phone_number']
    USERNAME_FIELD = 'username'
    objects = MyUserManager()

    def __str__(self) -> str:
        return f"{self.username}, {self.email}, id: {self.id}"
