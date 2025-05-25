#include "cdtest.h"


#include <assert.h>
#include <stdlib.h>

#include <scroll_menu.h>


#define MENU_ENTRY_COUNT (16)

static menu_entry_t _menu_entries[MENU_ENTRY_COUNT];

static         scroll_menu_t menu;


static smpc_peripheral_digital_t _digital;

static cdfs_filelist_t _filelist;

static uint16_t _frt_overflow_count = 0;

static void _menu_input(scroll_menu_t *menu);
static void _menu_update(scroll_menu_t *menu);
static void _menu_action(void *work, menu_entry_t *menu_entry);


void CDTestState_Init(void)
{
	/* Load the maximum number. We have to free the allocated filelist
         * entries, but since we never exit, we don't have to */
        cdfs_filelist_entry_t * const filelist_entries =
            cdfs_entries_alloc(-1);
        assert(filelist_entries != NULL);

        cdfs_config_default_set();
        cdfs_filelist_init(&_filelist, filelist_entries, -1);
        cdfs_filelist_root_read(&_filelist);

       
        scroll_menu_init(&menu);
        scroll_menu_input_set(&menu, _menu_input);
        scroll_menu_update_set(&menu, _menu_update);
        scroll_menu_entries_set(&menu, _menu_entries, MENU_ENTRY_COUNT);

        menu.view_height = MENU_ENTRY_COUNT;
        menu.top_index = 0;
        menu.bottom_index = _filelist.entries_count - 1;

        menu.flags = SCROLL_MENU_ENABLED | SCROLL_MENU_INPUT_ENABLED;
}


void CDTestState_Free(void)
{
}


void CDTestState_Update(void)
{
	                smpc_peripheral_process();
                smpc_peripheral_digital_port(1, &_digital);

                dbgio_printf("[H[2J");

                scroll_menu_update(&menu);
}


gamestate_t Get_CDTestState(void)
{
	return (gamestate_t)
	{
		.InitState = CDTestState_Init,
		.FreeState = CDTestState_Free,
		.UpdateState = CDTestState_Update,
	};
}




static void
_menu_input(scroll_menu_t *menu)
{
        if ((_digital.held.button.down) != 0) {
                scroll_menu_cursor_down(menu);
        } else if ((_digital.held.button.up) != 0) {
                scroll_menu_cursor_up(menu);
        } else if ((_digital.held.button.a) != 0) {
                scroll_menu_action_call(menu);
        }
}

static void
_menu_update(scroll_menu_t *menu)
{
        for (uint8_t i = 0; i < menu->view_height; i++) {
                menu_entry_t * const menu_entry = &_menu_entries[i];

                const uint32_t y = scroll_menu_local_cursor(menu) + i;

                char * const name = _filelist.entries[y].name;

                if ((y >= _filelist.entries_count) || (name == NULL) || (*name == '\0')) {
                        *menu_entry->label = '\0';
                        menu_entry->action_fn = NULL;

                        continue;
                }

                strncpy(menu_entry->label, name, sizeof(menu_entry->label));
                menu_entry->action_fn = _menu_action;
        }
}

static void
_menu_action(void *state_ptr, menu_entry_t *menu_entry __unused)
{
        scroll_menu_t *menu;
        menu = state_ptr;

        uint32_t i = scroll_menu_cursor(menu);

        cdfs_filelist_entry_t *file_entry;
        file_entry = &_filelist.entries[i];

        dbgio_printf("\n\nLoading %s, FAD: %li, %i sectors...\n",
            file_entry->name,
            file_entry->starting_fad,
            file_entry->sector_count);

        int ret __unused;
        ret = cd_block_sectors_read(file_entry->starting_fad, (void *)LWRAM(0), file_entry->size);
        assert(ret == 0);
}
