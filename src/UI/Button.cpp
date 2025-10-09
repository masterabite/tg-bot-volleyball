#include <UI/Button.h>

Button::Button(std::string _label, Menu* _menuNext) {

    tgButton = TgBot::InlineKeyboardButton::Ptr(new TgBot::InlineKeyboardButton);

    label                   = _label;
    name                    = label;
    tgButton->text          = label;
    tgButton->callbackData  = label;
    menuNext                = _menuNext;
}

Button::~Button() {
    printf("delete button %p\n", this);
}

TgBot::InlineKeyboardButton::Ptr Button::get_tg_button() {
    return tgButton;
}

std::string Button::get_name() {
    return name;
}

Menu* Button::get_menu_next() {
    return menuNext;
}