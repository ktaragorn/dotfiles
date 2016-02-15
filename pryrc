if defined?(PryDebugger)
  Pry.commands.alias_command 'c', 'continue'
  Pry.commands.alias_command 's', 'step'
  Pry.commands.alias_command 'n', 'next'
  Pry.commands.alias_command 'f', 'finish'
  Pry.commands.alias_command 'l', 'whereami'
  Pry.commands.alias_command 'q', 'exit-program'
  Pry.commands.alias_command 'bt', 'show-stack'
end

require "awesome_print"
if defined?(AwesomePrint)
  AwesomePrint.pry!
end
