from django.urls import path
from . import views

app_name = 'my_statistics'

urlpatterns = [
    path('', views.statistics, name='my_statistics'),
]
