#include "simple_logger.h"

#include "gfc_list.h"
#include "gfc_callbacks.h"

#include "gf2d_elements.h"
#include "gf2d_element_label.h"
#include "gf2d_element_entry.h"

#include "windows_common.h"

//Pause Button
int yes_no_free(Window *win)
{
    List *list;
    int count,i;
    Callback *callback;

    if (!win)return 0;
    if (!win->data)return 0;

    list = (List*)win->data;
    count = gfc_list_get_count(list);

    for (i = 0; i < count; i++)
    {
        callback = (Callback*)gfc_list_get_nth(list,i);
        if (callback)
        {
            gfc_callback_free(callback);
        }
    }

    gfc_list_delete(list);
    return 0;
}

int yes_no_update(Window *win,List *updateList)
{
    int i,count;
    Element *e;
    List *callbacks;
    Callback *callback;
    if (!win)return 0;
    if (!updateList)return 0;
    callbacks = (List*)win->data;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        if (!e)continue;
        switch(e->index)
        {
            case 51:
                callback = (Callback*)gfc_list_get_nth(callbacks,0);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
                break;
            case 52:
                callback = (Callback*)gfc_list_get_nth(callbacks,1);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
        }
    }
    return 0;
}

Window *window_yes_no(char *text, void(*onYes)(void *),void(*onNo)(void *),void *yesData,void *noData)
{
    Window *win;
    List *callbacks;
    win = gf2d_window_load("config/yes_no_window.json");
    if (!win)
    {
        slog("failed to load yes/no window");
        return NULL;
    }
    gf2d_element_label_set_text(gf2d_window_get_element_by_id(win,1),text);
    win->update = yes_no_update;
    win->free_data = yes_no_free;
    callbacks = gfc_list_new();
    callbacks = gfc_list_append(callbacks,gfc_callback_new(onYes,yesData));
    callbacks = gfc_list_append(callbacks,gfc_callback_new(onNo,noData));
    win->data = callbacks;
    return win;
}

//Main Menu
int main_menu_free(Window *win)
{
    List *list;
    int count,i;
    Callback *callback;

    if (!win)return 0;
    if (!win->data)return 0;

    list = (List*)win->data;
    count = gfc_list_get_count(list);

    for (i = 0; i < count; i++)
    {
        callback = (Callback*)gfc_list_get_nth(list,i);
        if (callback)
        {
            gfc_callback_free(callback);
        }
    }

    gfc_list_delete(list);
    return 0;
}

int main_menu_update(Window *win, List *updateList)
{
    int i,count;
    Element *e;
    List *callbacks;
    Callback *callback;
    if (!win)return 0;
    if (!updateList)return 0;
    callbacks = (List*)win->data;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        if (!e)continue;
        switch(e->index)
        {
            case 51:
                callback = (Callback*)gfc_list_get_nth(callbacks,0);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
                break;
            case 52:
                callback = (Callback*)gfc_list_get_nth(callbacks,1);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
        }
    }
    return 0;
}

Window *window_main_menu(char *text, void(*onYes)(void *),void(*onNo)(void *),void *yesData,void *noData)
{
    Window *win;
    List *callbacks;
    win = gf2d_window_load("config/main_menu_window.json");
    if (!win)
    {
        slog("failed to load main menu window");
        return NULL;
    }
    gf2d_element_label_set_text(gf2d_window_get_element_by_id(win, 1), text);
    win->update = main_menu_update;
    win->free_data = main_menu_free;
    callbacks = gfc_list_new();
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onYes, yesData));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onNo, noData));
    win->data = callbacks;
    return win;
}

//Level selector
int level_selector_free(Window *win)
{
    List *list;
    int count,i;
    Callback *callback;

    if (!win)return 0;
    if (!win->data)return 0;

    list = (List*)win->data;
    count = gfc_list_get_count(list);

    for (i = 0; i < count; i++)
    {
        callback = (Callback*)gfc_list_get_nth(list,i);
        if (callback)
        {
            gfc_callback_free(callback);
        }
    }

    gfc_list_delete(list);
    return 0;
}

int level_selector_update(Window *win, List *updateList)
{
    int i,count;
    Element *e;
    List *callbacks;
    Callback *callback;
    if (!win)return 0;
    if (!updateList)return 0;
    callbacks = (List*)win->data;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        if (!e)continue;
        switch(e->index)
        {
            case 51:
                callback = (Callback*)gfc_list_get_nth(callbacks,0);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
                break;
            case 52:
                callback = (Callback*)gfc_list_get_nth(callbacks,1);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
            case 53:
                callback = (Callback*)gfc_list_get_nth(callbacks,2);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
        }
    }
    return 0;
}

Window *window_level_selector(char *text, void(*onOne)(void *),void(*onTwo)(void *),void(*onThree)(void *),void(*onFour)(void *),void(*onFive)(void *),void *oneData,void *twoData,void *threeData,void *fourData,void *fiveData, int secret)
{
    Window *win;
    List *callbacks;
    if (secret == 1)win = gf2d_window_load("config/s_level_selector_window.json");
    else win = gf2d_window_load("config/level_selector_window.json");
    if (!win)
    {
        slog("failed to load level selector window");
        return NULL;
    }
    gf2d_element_label_set_text(gf2d_window_get_element_by_id(win, 1), text);
    win->update = level_selector_update;
    win->free_data = level_selector_free;
    callbacks = gfc_list_new();
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onOne, oneData));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onTwo, twoData));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onThree, threeData));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onFour, onFour));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onFive, onFive));
    win->data = callbacks;
    return win;
}

//Penguin selector
int penguin_selector_free(Window *win)
{
    List *list;
    int count,i;
    Callback *callback;

    if (!win)return 0;
    if (!win->data)return 0;

    list = (List*)win->data;
    count = gfc_list_get_count(list);

    for (i = 0; i < count; i++)
    {
        callback = (Callback*)gfc_list_get_nth(list,i);
        if (callback)
        {
            gfc_callback_free(callback);
        }
    }

    gfc_list_delete(list);
    return 0;
}

int penguin_selector_update(Window *win, List *updateList)
{
    int i,count;
    Element *e;
    List *callbacks;
    Callback *callback;
    if (!win)return 0;
    if (!updateList)return 0;
    callbacks = (List*)win->data;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        if (!e)continue;
        switch(e->index)
        {
            case 51:
                callback = (Callback*)gfc_list_get_nth(callbacks,0);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
                break;
            case 52:
                callback = (Callback*)gfc_list_get_nth(callbacks,1);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
            case 53:
                callback = (Callback*)gfc_list_get_nth(callbacks,2);
                if (callback)
                {
                    gfc_callback_call(callback);
                }
                gf2d_window_free(win);
                return 1;
        }
    }
    return 0;
}

Window *window_penguin_selector(char *text, void(*onOne)(void *),void(*onTwo)(void *),void(*onThree)(void *),void(*onFour)(void *),void *oneData,void *twoData,void *threeData,void *fourData)
{
    Window *win;
    List *callbacks;
    win = gf2d_window_load("config/penguin_selector_window.json");
    if (!win)
    {
        slog("failed to load penguin selector window");
        return NULL;
    }
    gf2d_element_label_set_text(gf2d_window_get_element_by_id(win, 1), text);
    win->update = penguin_selector_update;
    win->free_data = penguin_selector_free;
    callbacks = gfc_list_new();
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onOne, oneData));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onTwo, twoData));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onThree, threeData));
    callbacks = gfc_list_append(callbacks, gfc_callback_new(onFour, fourData));
    win->data = callbacks;
    return win;
}
//Common
Window *window_text_entry(char *question, char *defaultText, size_t length, void(*onOk)(void *),void(*onCancel)(void *))
{
    Window *win;
    List *callbacks;
    win = gf2d_window_load("config/text_entry_window.json");
    if (!win)
    {
        slog("failed to load yes/no window");
        return NULL;
    }
    gf2d_element_label_set_text(gf2d_window_get_element_by_id(win,1),question);
    gf2d_element_entry_set_text_pointer(gf2d_window_get_element_by_id(win,2),defaultText,length);
    win->update = yes_no_update;
    win->free_data = yes_no_free;
    callbacks = gfc_list_new();
    callbacks = gfc_list_append(callbacks,gfc_callback_new(onOk,defaultText));
    callbacks = gfc_list_append(callbacks,gfc_callback_new(onCancel,defaultText));
    win->data = callbacks;
    return win;
}

/*eol@eof*/