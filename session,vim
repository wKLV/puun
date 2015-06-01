let SessionLoad = 1
if &cp | set nocp | endif
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +22 .vimrc.local
badd +3 vimwiki/index.wiki
badd +1 vimwiki/Project\ Gutenberg.wiki
badd +1 vimwiki/shit
badd +1 vimwiki/diary/diary.wiki
badd +1 vimwiki/Tasks\ --\ things.wiki
badd +1 vimwiki/Secret\ projects.wiki
badd +3 vimwiki/puun.wiki
badd +55 pjs/puun/src/texttest/tt.c
badd +5 pjs/puun/src/puun/gf/sprite.h
badd +9 pjs/puun/src/puun/gf/sprite.c
badd +13 pjs/puun/src/firsty/secondy.c
badd +1 ../firsty/secondy.c
badd +18 pjs/puun/src/puun/gf/square2.c
badd +0 pjs/puun/src/puun/gf/text.c
badd +0 pjs/puun/src/puun/types.h
badd +63 pjs/puun/src/sprites/sprite.c
badd +0 pjs/puun/build.sh
badd +0 pjs/puun/src/sprites/sdl_sprites.c
badd +0 pjs/puun/src/puun/gf/gl_help.c
badd +0 pjs/puun/src/puun/gf/square.h
silent! argdel *
edit vimwiki/puun.wiki
set splitbelow splitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 44 - ((31 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
44
normal! 018|
lcd ~/vimwiki
tabedit ~/pjs/puun/build.sh
set splitbelow splitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 9 - ((8 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
9
normal! 083|
lcd ~/pjs/puun
tabedit ~/pjs/puun/src/puun/gf/square2.c
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 69 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 70 + 70) / 140)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 55 - ((4 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
55
normal! 014|
lcd ~/pjs/puun/src/puun/gf
wincmd w
argglobal
edit ~/pjs/puun/src/puun/gf/square.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 7 - ((6 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
7
normal! 013|
lcd ~/pjs/puun/src/puun/gf
wincmd w
exe 'vert 1resize ' . ((&columns * 69 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 70 + 70) / 140)
tabedit ~/pjs/puun/src/puun/gf/sprite.h
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 69 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 70 + 70) / 140)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 16 - ((15 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
16
normal! 017|
lcd ~/pjs/puun/src/puun/gf
wincmd w
argglobal
edit ~/pjs/puun/src/puun/gf/sprite.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 54 - ((20 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
54
normal! 09|
lcd ~/pjs/puun/src/puun/gf
wincmd w
exe 'vert 1resize ' . ((&columns * 69 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 70 + 70) / 140)
tabedit ~/pjs/puun/src/sprites/sprite.c
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 69 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 70 + 70) / 140)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 63 - ((28 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
63
normal! 035|
lcd ~/pjs/puun/src/sprites
wincmd w
argglobal
edit ~/pjs/puun/src/sprites/sdl_sprites.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 27 - ((26 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
27
normal! 0
lcd ~/pjs/puun/src/sprites
wincmd w
exe 'vert 1resize ' . ((&columns * 69 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 70 + 70) / 140)
tabedit ~/pjs/puun/src/puun/gf/gl_help.c
set splitbelow splitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 150 - ((46 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
150
normal! 06|
lcd ~/pjs/puun/src/puun/gf
tabedit ~/pjs/puun/src/puun/gf/text.c
set splitbelow splitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 45 - ((44 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
45
normal! 09|
lcd ~/pjs/puun/src/puun/gf
tabedit ~/pjs/puun/src/puun/types.h
set splitbelow splitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 36 - ((33 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
36
normal! 017|
lcd ~/pjs/puun/src/puun
tabnew
set splitbelow splitright
wincmd t
set winheight=1 winwidth=1
argglobal
enew
file ~/pjs/puun/NERD_tree_4
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal nofen
lcd ~/pjs/puun
tabnext 9
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
