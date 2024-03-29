from django.db import models
from login.models import MyUser
from django.core.validators import MinValueValidator, MaxValueValidator
# Create your models here.


class News(models.Model):
    name = models.CharField(max_length=100)
    brief_description = models.CharField(max_length=200)
    url = models.URLField()


class Faq(models.Model):
    question = models.CharField(max_length=100)
    answer = models.CharField(max_length=200)
    post_date = models.DateField(auto_now_add=True)


class Review(models.Model):
    customer = models.ForeignKey(MyUser, on_delete=models.CASCADE)
    content = models.TextField()
    rating = models.IntegerField(
        validators=[MinValueValidator(1), MaxValueValidator(5)])
    created_date = models.DateField(auto_now_add=True)
    updated_date = models.DateField(auto_now=True)


class PromoCode(models.Model):
    promo_code = models.IntegerField(unique=True,
                                     validators=[MinValueValidator(1), MaxValueValidator(10000)])
    name = models.CharField(max_length=100)
    discount_size = models.IntegerField(
        validators=[MinValueValidator(1), MaxValueValidator(100)])
    active = models.BooleanField(default=True)


class BannerChangeTimer(models.Model):
    milliseconds = models.IntegerField(
        validators=[MinValueValidator(1000), MaxValueValidator(30000)])
