from django.urls import path
from . import views

urlpatterns = [
    path('add/', views.add_plant_data, name='add_plant_data'),
]
