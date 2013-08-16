import sublime, sublime_plugin
import os
import sys
import json
import codecs

# clist_dict = {}
if not 'clist_dict' in globals(): clist_dict = {}


class History():

    def __init__(self, key, view):
        self.key = key
        self.view = view
        self.update()

    def update(self):
        self.file_name = self.view.file_name()
        self.point = list(self.view.sel())[0].a
        print self.file_name


# Change List object
class CList():
    LIST_LIMIT = 50
    key_counter = 0
    pointer = -1
    history_list = []
    try_again_index = 0
    goto_in_progress = False

    def __init__(self,window):
        self.window = window #needed?

    def push_key(self,view):
        region_list = list(view.sel())
        if not region_list: return

        if self.history_list:
            last_sel = view.get_regions(self.history_list[-1].key)
            # dont push key if no jump
            if last_sel == region_list: return

            # delete last key if same line
            if len(last_sel) == len(region_list):
                if all([view.rowcol(i.begin())[0]==view.rowcol(j.begin())[0] for i,j in zip(last_sel, region_list)]):
                    self.key_counter -= 1
                    self.history_list.pop(-1)

        self.pointer = -1
        key = self.generate_key()
        view.erase_regions(key)
        view.add_regions(key, region_list ,"")
        self.history_list.append(History(key,view))

    def generate_key(self):
        self.key_counter += 1
        self.key_counter %= self.LIST_LIMIT * 2

        # if number of keys doubles the LIST_LIMIT, reload the keys
        if self.key_counter == 0:
            self.reload_keys()
            self.key_counter += 1
        return "cl"+str(self.key_counter)

    def reload_keys(self, sel_list=None):
        view = self.view
        if not sel_list: sel_list = [self.view.get_regions(history.key) for history in self.history_list]
        for i,sel in enumerate(sel_list):
            view.erase_regions("cl"+str(i+1))
            view.add_regions("cl"+str(i+1), sel, "")
        self.history_list = [History("cl"+str(i+1)) for i in range(len(sel_list))]
        # print(self.history_list)
        self.key_counter = len(sel_list)


    def trim_keys(self):
        if len(self.history_list) > self.LIST_LIMIT:
            for i in range(0, len(self.history_list) - self.LIST_LIMIT):
                key = self.history_list[i].key
                view = self.history_list[i].view
                view.erase_regions(key)
            del self.history_list[: len(self.history_list) - self.LIST_LIMIT]

    def remove_empty_keys(self):
        new_key_list = []
        for history in self.history_list:
            key = history.key
            view = history.view
            # print(view.get_regions(key))
            if view.get_regions(key):
                new_key_list.append(history)
            else:
                view.erase_regions(key)
        self.history_list = new_key_list

    def update_head(self):
        self.history_list[-1].update()

    def try_again(self):
        old = self.try_again_index != 0
        if(old):
            self.goto(self.try_again_index)
        self.try_again_index = 0
        self.goto_in_progress = False
        return old

    def goto(self, index):
        # print(self.history_list)
        if index>=0 or index< -len(self.history_list): return

        self.goto_in_progress = True
        view = sublime.active_window().open_file(self.history_list[index].file_name)
        if view.is_loading():
            self.try_again_index = index;
            return

        self.pointer = index
        point = self.history_list[index].point
        #print "Going to index - %d/%d file - %s, point - %d" %(index, len(self.history_list), self.history_list[index].file_name, self.history_list[index].point)
        #sel = view.get_regions(self.history_list[index].key)
        view.sel().clear()
        view.show_at_center(point)#sel[0])
        sublime.active_window().focus_view(view)
        #for s in sel[1:]:
        #    view.sel().add(s)
        view.sel().add(sublime.Region(point,point))
        self.goto_in_progress = False


def load_jsonfile():
    jsonFilepath = os.path.join(sublime.packages_path(), 'User', 'ChangeList.json')
    if os.path.exists(jsonFilepath):
        jsonFile = codecs.open(jsonFilepath, "r+", encoding="utf-8")
        try:
            data = json.load(jsonFile)
        except:
            print("Not json file!")
            data = {}
        jsonFile.close()
    else:
        jsonFile = codecs.open(jsonFilepath, "w+", encoding="utf-8")
        data = {}
        jsonFile.close()
    return data

def save_jsonfile(data):
    jsonFilepath = os.path.join(sublime.packages_path(), 'User', 'ChangeList.json')
    jsonFile = codecs.open(jsonFilepath, "w+", encoding="utf-8")
    jsonFile.write(json.dumps(data, ensure_ascii=False))
    jsonFile.close()

def remove_jsonfile():
    jsonFilepath = os.path.join(sublime.packages_path(), 'User', 'ChangeList.json')
    if os.path.exists(jsonFilepath): os.remove(jsonFilepath)

def get_clist(window , reset =False):
    global clist_dict
    wid = window.id()
    # vname = view.file_name()
    if wid in clist_dict and not reset:
        this_clist = clist_dict[wid]
    else:
        this_clist = CList(window)
        clist_dict[wid] = this_clist
    return this_clist

    # if not hasattr(get_clist, "one_true_list") or reset: #(1)
    #     get_clist.one_true_list=CList()
        
    # return get_clist.one_true_list
    # not sure for the need for the json save-load so disabling
    # global clist_dict
    # vid = view.id()
    # vname = view.file_name()
    # if vid in clist_dict:
    #     this_clist = clist_dict[vid]
    # else:
    #     this_clist = CList(view)
    #     clist_dict[vid] = this_clist
    #     data = load_jsonfile()
    #     f = lambda s: sublime.Region(int(s[0]),int(s[1])) if len(s)==2 else sublime.Region(int(s[0]),int(s[0]))
    #     try:
    #         if vname in data:
    #             print("Reloading keys...")
    #             sel_list = [[f(s.split(",")) for s in sel.split(":")] for sel in data[vname]['history'].split("|")]
    #             this_clist.reload_keys(sel_list)
    #     except:
    #         print("Reload keys failed!")
    # return this_clist

class CListener(sublime_plugin.EventListener):
    def on_modified(self, view):
        if view.is_scratch() or view.settings().get('is_widget'): return
        this_clist = get_clist(sublime.active_window())
        this_clist.remove_empty_keys()
        this_clist.push_key(view)
        this_clist.trim_keys()
        # print(this_clist, this_clist.history_list)
        # for key in this_clist.history_list:
        #     print(view.get_regions(key))

    # fixed this but not sure if it actually fixed what it is meant to do.
    def on_post_save(self, view):
        this_clist = get_clist(sublime.active_window())
        vname = view.file_name()
        data = load_jsonfile()
        f = lambda s: str(s.begin())+","+str(s.end()) if s.begin()!=s.end() else str(s.begin())
        data[vname] =  {"history": "|".join([":".join([f(s) for s in view.get_regions(history.key)]) for history in this_clist.history_list])}
        save_jsonfile(data)

    def on_close(self, view):
        vid = view.id()
        if vid in clist_dict: clist_dict.pop(vid)

    def on_load(self,v):
        get_clist(sublime.active_window()).try_again()
        get_clist(sublime.active_window()).update_head()

    def on_activated(self,v):
        if not get_clist(sublime.active_window()).goto_in_progress:
            get_clist(sublime.active_window()).push_key(v)

class JumpToChange(sublime_plugin.TextCommand):
    def run(self, _, **kwargs):
        view = self.view
        # reset vintageous action
        try:
            vintage = view.settings().get('vintage')
            vintage['action'] = None
            view.settings().set('vintage', vintage)
        except:
            pass

        if view.is_scratch() or view.settings().get('is_widget'): return
        this_clist = get_clist(sublime.active_window())
        if not this_clist.history_list: return
        if 'move' in kwargs:
            move = kwargs['move']
            #not sure exactly what this is doing.. possibly something to do with the per-file lists
            #if move == -1 and this_clist.pointer == -1 # and view.get_regions(this_clist.history_list[-1].key) != list(view.sel()):
                #move = 0
            index = this_clist.pointer+move
        elif 'index' in kwargs:
            index = kwargs['index']
        else:
            return
        if index>=0 or index< -len(this_clist.history_list): return
        # print(len(this_clist.history_list))
        this_clist.goto(index)
        # to reactivate cursor
        view.run_command("move", {"by": "characters", "forward" : False})
        view.run_command("move", {"by": "characters", "forward" : True})
        sublime.status_message("@[%s]" % str(-index-1))

class ShowChangeList(sublime_plugin.WindowCommand):
    def run(self):
        view = self.window.active_view()
        if view.is_scratch() or view.settings().get('is_widget'): return
        this_clist = get_clist(self.window)
        if not this_clist.history_list: return
        def f(i,history):
            #print history.file_name
            view = history.view
            begin = view.get_regions(key.key)[0].begin()
            return "[%2d]  %s : %3d  -  %s" % (i,os.path.basename(history.file_name) if history.file_name else "Unknown",view.rowcol(begin)[0]+1, view.substr(view.line(begin)))
        self.window.show_quick_panel([ f(i,key)
                    for i,key in enumerate(reversed(this_clist.history_list))], self.on_done)

    def on_done(self, action):
        view = self.window.active_view()
        this_clist = get_clist(self.window)
        if action==-1: return
        # print(-action-1)
        view.run_command("jump_to_change", {"index" : -action-1})

class MaintainChangeList(sublime_plugin.WindowCommand):

    def show_quick_panel(self, options, done):
        sublime.set_timeout(lambda: self.window.show_quick_panel(options, done), 10)

    def run(self):
        view = self.window.active_view()
        if view.is_scratch() or view.settings().get('is_widget'): return
        try:
            fname = os.path.basename(view.file_name())
        except:
            fname = "untitled"
        self.show_quick_panel(
            ["Rebuild History", 
            #"Clear History - "+fname,  Loses meaning now that the history is interspersed
            "Clear All History"], self.confirm)

    def confirm(self, action):
        if action<0: return
        self.action = action
        self.show_quick_panel(["Cancel", "Apply"], self.on_done)

    def on_done(self, confirm):
        view = self.window.active_view()
        if confirm<=0:
            self.run()
            return

        global clist_dict
        action = self.action
        if action==0:
            data = load_jsonfile()
            for item in [item for item in data if not os.path.exists(item)]:
                data.pop(item)
            save_jsonfile(data)
            sublime.status_message("Change List History is rebuilt successfully.")
        # elif action==1:
        #     vid = view.id()
        #     vname = view.file_name()
        #     if vid in clist_dict: clist_dict.pop(vid)
        #     if vname:
        #         data = load_jsonfile()
        #         if vname in data:
        #             data.pop(vname)
        #             save_jsonfile(data)
        #     sublime.status_message("Change List (this file) is cleared successfully.")
        elif action== 1:#2:
            clist_dict = {}
            get_clist(sublime.active_window(),True)
            remove_jsonfile()
            sublime.status_message("Change List of all files in this window is cleared successfully.")
