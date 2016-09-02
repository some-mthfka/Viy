"Used for NeoSFML GUI

function! neosfml#PicCopyAndInc(reps)
    let c = 1
    while c <= a:reps
        execute "normal! yyp$bb\<C-a>"
        let c += 1
    endwhile
endfunction

function! neosfml#PicSize()
    return split(getline('.'), '|')[-3] - 1
endfunction

function! neosfml#PicExpand()
    call neosfml#PicCopyAndInc(neosfml#PicSize())
endfunction

function! neosfml#PicExpandAll()
    "don't forget to restore register @a
    execute "g/<|\\(\\w\\| \\)\\+\\.\\w\\+|\\d*|0|>/call neosfml#PicExpand()"
endfunction

function! neosfml#PicContractAll()
    silent execute "g/<|\\(\\w\\| \\)\\+\\.\\w\\+|\\d\\{1,2}|\\d*|>/d_"
endfunction
