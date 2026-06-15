cmake_minimum_required(VERSION 3.13)

include(template-project-sanitizers-regular)

set(EXTRA_COMPILE_OPTIONS 
  ${EXTRA_COMPILE_OPTIONS}
  -fsanitize=thread 
  -fnon-call-exceptions
)

set(
  EXTRA_LINK_OPTIONS 
  ${EXTRA_LINK_OPTIONS} 
  -fsanitize=thread
)
