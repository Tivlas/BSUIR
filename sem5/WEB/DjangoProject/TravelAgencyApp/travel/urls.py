from django.urls import path
from . import views

app_name = 'travel'

urlpatterns = [
    path('', views.list_trips, name='list_trips'),
    path('<int:id>',views.trip_details,name='trip_details'),
    path('edit_trip/<int:id>/',views.edit_trip,name='edit_trip'),
    path('delete_trip/<int:id>/',views.delete_trip,name='delete_trip'),
    path('create_trip/', views.create_trip, name='create_trip'),
    path('<str:trip_country_name>/', views.list_trips,
         name='list_trips_by_country'
         ),
]
