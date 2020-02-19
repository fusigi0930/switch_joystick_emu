count = 0
thread_c = 0

function thread_func()
        while true do
                print("thread: ", thread_c)
                wait(100)
                thread_c = thread_c + 1
        end
end

run_thread("thread_func")
while true do
        print("count: ", count)
        count = count+1
        wait(300)
end
