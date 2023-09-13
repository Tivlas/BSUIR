from django.db import models

# Create your models here.
class News(models.Model):
    name = models.CharField(max_length=100)
    brief_description = models.CharField(max_length=200)
    url = models.URLField()

class Faq(models.Model):
    question =  models.CharField(max_length=100)
    answer = models.CharField(max_length=200)
    post_date = models.DateField(auto_now_add=True)