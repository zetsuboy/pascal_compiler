using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace HW1
{

    public partial class MainPage : ContentPage
    {
        public ObservableCollection<Item> items = new ObservableCollection<Item>();

        public MainPage()
        {
            InitializeComponent();
            
            BindableLayout.SetItemsSource(items_container, items);
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            var itemPage = new AddItemPage();
            itemPage.Disappearing += (_, __) => {
                if (itemPage.SelectedItem != null)
                {
                    var selectedItem = itemPage.SelectedItem;
                    items.Where(x => x.item_name == selectedItem.item_name).ToList().ForEach(x =>
                    {
                        selectedItem.item_count += x.item_count;
                        items.Remove(x);
                    });
                    items.Add(selectedItem);
                }
            };
            Navigation.PushAsync(itemPage);
        }

        private void Button_Clicked_1(object sender, EventArgs e)
        {
            var button = sender as Button;
            foreach(Item i in items)
            {
                if(i.item_id.ToString() == button.ClassId)
                {
                    items.Remove(i);
                    break;
                }
                
            }
        }

        private async void OrderButton_Clicked(object sender, EventArgs e)
        {
            if (items.Count == 0)
            {
                await DisplayAlert("Order is empty", "Add item to order", "Ok");
            }
            else
            {
                var result = await DisplayAlert("Confirm", "Order description", "Agree", "Disagree");
                if (result)
                {
                    items.Clear();
                    await DisplayAlert("Successeful", "Order description", "Ok");
                }
            }
        }
    }
}
