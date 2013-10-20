import sublime
import sublime_plugin
import re, sys

if not 'dict' in globals(): dict = {}
if not 'hist' in globals(): hist = []

def input_parser(user_input):
    m = re.match(r"(.+)/([lcr*()0-9]*)(f[0-9]*)?", user_input)

    if m and (m.group(2) or m.group(3)):
        regex = m.group(1)
        option = m.group(2)
        f = m.group(3)
    else:
        print("No options!")
        return [user_input, [['l', 1]], 0]

    try:
        # for option
        rParan = re.compile(r"\(([^())]*)\)\*([0-9]+)")
        while True:
            if not rParan.search(option): break
            for r in rParan.finditer(option):
                option = option.replace(r.group(0), r.group(1)*int(r.group(2)),1)

        for r in re.finditer(r"([lcr][0-9]*)\*([0-9]+)", option):
            option = option.replace(r.group(0), r.group(1)*int(r.group(2)),1)

        option = re.findall(r"[lcr][0-9]*", option)
        option = list(map(lambda x: [x[0], 1] if len(x)==1 else [x[0], int(x[1:])], option))
        option = option if option else [['l', 1]]

        # for f
        f = 0 if not f else 1 if len(f)==1 else int(f[1:])
    except:
        [regex, option ,f]= [user_input, [['l', 1]], 0]

    return [regex, option ,f]

def update_colwidth(colwidth, content, option):
    thiscolwidth = [len(s.strip(" ")) for s in content]
    for i,w in enumerate(thiscolwidth):
        if i<len(colwidth):
            colwidth[i] = max(colwidth[i], w)
        else:
            colwidth.append(w)

def get_named_pattern(user_input):
    patterns = sublime.load_settings('AlignTab.sublime-settings').get('named_patterns', {})
    user_input = patterns[user_input] if user_input in patterns else user_input
    user_input = hist[-1] if hist and user_input == 'last_rexp' else user_input
    return user_input

class AlignTabCommand(sublime_plugin.TextCommand):

    def run(self, edit, user_input=None, mode=False):
        if not user_input:
            v = self.view.window().show_input_panel('Align with regex:', '',
                    lambda x: self.view.run_command("align_tab",{"user_input":x, "mode":mode}), None, None)
            v.set_syntax_file('Packages/AlignTab/AlignTab.hidden-tmLanguage')
            v.settings().set('gutter', False)
            v.settings().set('rulers', [])
        else:
            self.align_tab(edit, user_input, mode)

    def get_line_content(self, regex, f, row):
        view = self.view
        line = view.line(view.text_point(row,0))
        return [s for s in re.split(regex,view.substr(line),f)]

    def expand_sel(self, regex, option, f, rows, colwidth):
        view = self.view
        lastrow = view.rowcol(view.size())[0]
        for sel in view.sel():
            for line in view.lines(sel):
                thisrow = view.rowcol(line.begin())[0]
                if (thisrow in rows): continue
                content = self.get_line_content(regex, f, thisrow)
                if len(content)<=1: continue
                update_colwidth(colwidth, content, option)
                rows.append(thisrow)

            if sel.begin()==sel.end():
                thisrow = view.rowcol(sel.begin())[0]
                if not (thisrow in rows): continue
                beginrow = endrow = thisrow
                while endrow+1<=lastrow and not (endrow+1 in rows):
                    content = self.get_line_content(regex, f, endrow+1)
                    if len(content)<=1: break
                    update_colwidth(colwidth, content, option)
                    endrow = endrow+1
                    rows.append(endrow)
                while beginrow-1>=0 and not (beginrow-1 in rows):
                    content = self.get_line_content(regex, f, beginrow-1)
                    if len(content)<=1: break
                    update_colwidth(colwidth, content, option)
                    beginrow = beginrow-1
                    rows.append(beginrow)


    def align_tab(self, edit, user_input, mode):
        view = self.view
        vid = view.id()

        # insert history and reset index
        if not hist or (user_input!= hist[-1] and user_input!= "last_rexp"): hist.append(user_input)
        AlignTabHistory.index = None

        user_input = get_named_pattern(user_input)

        [regex, option, f] = input_parser(user_input)
        regex = '(' + regex + ')'

        rows = []
        colwidth = []
        self.expand_sel(regex, option, f , rows, colwidth)
        rows = sorted(set(rows))
        global dict

        if rows:
            if mode == True:
                dict[vid] = {"mode": 1}
                view.set_status("AlignTab", "[Table Mode]")
        else:
            if mode == True:
                dict[vid] = {"mode": 0}
                view.set_status("AlignTab", "")
            return

        spacebefore = min([re.match("^(\s*)",
                view.substr(view.line(view.text_point(row,0)))).group(1) for row in rows])

        for row in reversed(rows):
            content = self.get_line_content(regex, f, row)
            # the last col
            pt = view.line(view.text_point(row, 0)).end()
            for i, c in reversed(list(enumerate(content))):
                op = option[i % len(option)]
                # lenc = len(c)
                pt = pt-len(c)
                lenc = len(c.strip(' '))
                se = len(c) - len(c.rstrip(' '))
                sb = len(c)-lenc-se
                if op[0] == "l":
                    fill = colwidth[i]-lenc+op[1] if i != len(content)-1 else 0
                    oldpt = [min(s.end()-sb,pt+lenc+fill) for s in view.sel() if s.empty() and pt+sb+lenc<=s.end()<=pt+sb+lenc+se]
                    view.erase(edit, sublime.Region(pt,pt+sb))
                    view.erase(edit, sublime.Region(pt+lenc,pt+lenc+se))
                    view.insert(edit, pt+lenc, " "*(fill))
                    if oldpt:
                        view.sel().subtract(sublime.Region(pt+lenc, pt+lenc+fill))

                        for s in [sublime.Region(b,b) for b in oldpt]: view.sel().add(s)
                        # ST 2 does not support add_all(a list)
                        # view.sel().add_all([sublime.Region(b,b) for b in oldpt])

                if op[0] == "r":
                    fill = colwidth[i]-lenc
                    view.erase(edit, sublime.Region(pt,pt+sb))
                    view.erase(edit, sublime.Region(pt+lenc,pt+lenc+se))
                    if i != len(content)-1: view.insert(edit, pt+lenc, " "*op[1])
                    view.insert(edit, pt, " "*fill)

                if op[0] == "c":
                    fill = colwidth[i]-lenc
                    lfill = int(fill/2)
                    rfill = fill-int(fill/2)+op[1] if i != len(content)-1 else 0
                    oldpt = [min(s.end()-sb+lfill,pt+lenc+lfill+rfill) for s in view.sel() if s.empty() and pt+sb+lenc<=s.end()<=pt+sb+lenc+se]
                    view.erase(edit, sublime.Region(pt,pt+sb))
                    view.erase(edit, sublime.Region(pt+lenc,pt+lenc+se))
                    view.insert(edit, pt, " "*lfill)
                    view.insert(edit, pt+lfill+lenc, " "*rfill)
                    if oldpt:
                        view.sel().subtract(sublime.Region(pt+lfill+lenc, pt+lenc+lfill+rfill))
                        for s in [sublime.Region(b,b) for b in oldpt]: view.sel().add(s)


            view.insert(edit, view.text_point(row,0), spacebefore)

class AlignTabClearMode(sublime_plugin.TextCommand):
    def run(self, edit):
        view = self.view
        if view.is_scratch() or view.settings().get('is_widget'): return
        vid = view.id()
        global dict
        print("Clear Table Mode!")
        if vid in dict:
            dict[vid]["mode"] = 0
            view.set_status("AlignTab", "")

class AlignTabUndo(sublime_plugin.WindowCommand):
    def run(self):
        view = self.window.active_view()
        if view.is_scratch() or view.settings().get('is_widget'): return
        vid = view.id()
        global dict
        if vid in dict:
            dict[vid]["mode"] = 0
            view.run_command("undo")
            view.run_command("undo")
            dict[vid]["mode"] = 1

class AlignTabUpdater(sublime_plugin.EventListener):
    # Table mode
    def on_modified(self, view):
        if view.is_scratch() or view.settings().get('is_widget'): return
        cmdhist = view.command_history(0)
        if cmdhist[0] not in ["insert", "left_delete", "right_delete", "paste", "cut"]: return
        if cmdhist[0] == "insert" and cmdhist[1] == {'characters': ' '}: return
        vid = view.id()
        global dict
        if vid in dict:
            if dict[vid]["mode"] == 1:
                print("table mode:", view.command_history(0))
                view.run_command("align_tab", {"user_input": "last_rexp", "mode": True})

    def on_query_context(self, view, key, operator, operand, match_all):
        if view.is_scratch() or view.settings().get('is_widget'): return
        vid = view.id()
        if key == 'align_tab_mode':
            global dict
            if vid in dict:
                return dict[vid]["mode"] == 1
            else:
                return False

    # restore History index
    def on_deactivated(self, view):
        if view.score_selector(0, 'text.aligntab') > 0:
            AlignTabHistory.index = None

# VintageEX teaches me the following
class AlignTabHistory(sublime_plugin.TextCommand):
    index = None
    def run(self, edit, backwards=False):
        if AlignTabHistory.index is None:
            AlignTabHistory.index = -1 if backwards else 0
        else:
            AlignTabHistory.index += -1 if backwards else 1

        if AlignTabHistory.index == len(hist) or \
            AlignTabHistory.index < -len(hist):
                AlignTabHistory.index = -1 if backwards else 0

        self.view.erase(edit, sublime.Region(0, self.view.size()))
        self.view.insert(edit, 0, hist[AlignTabHistory.index])

