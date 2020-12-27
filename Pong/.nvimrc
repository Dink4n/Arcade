let build_type = "Debug"
let bin = "Pong"

nmap <Leader>pb         :exe printf("Dispatch make -s -C temp/%s", build_type)<CR>
nmap <Leader>pr         :exe printf("Dispatch temp/%s/%s", build_type, bin)<CR>
nmap <Leader>pg         :exe printf("Dispatch cmake -S . -B temp/%s -DCMAKE_BUILD_TYPE=%s", build_type, build_type)<CR>
nmap <Leader>pc         :Dispatch rm -rf temp/*<CR>
