#!/usr/bin/env ruby
src_path, dest_host, dest_port, dest_path = ARGV
port ||= 22
require "ostruct"

class LastSyncList
  def initialize src_path
    @file = File.join(src_path, ".last_sync_list")
  end

  def read
    File.readlines(@file).map(&:chomp)
  rescue Errno::ENOENT
    []
  end

  def write list
    File.write(@file, Array(list).join("\n"))
  end
end

def file_list_cmd path
  "cd \"#{path}\"; find . -type f"
end

def current_list_remote dest
  files = ssh_command dest.host, dest.port,file_list_cmd(dest.path)
  files.split("\n")
end

def ssh_command host, port, command
  `ssh #{host} -p #{port} \"#{command}\"`
end

def delete_remote_deleted_files src_path, list
  list.each do |file|
    puts "Deleting #{file}"
    File.delete(File.join(src_path, file))
  end
end

def rsync_push_new_files src, dest
  # ignore existing to optimize for android transfer where i think modification time or something is not being set
  # Files are not expected to be modified on the remote, so this should suffice
  # -W to optimize for large files
  system("rsync -rvzW --ignore-existing --progress -e \"ssh -p #{dest.port}\" \"#{src}\" \"#{dest.host}:#{dest.path}\"")
end

def current_list_local path
  files = `#{file_list_cmd path}`
  files.split("\n")
end

dest = OpenStruct.new(host: dest_host, port: dest_port, path: dest_path)
lsl = LastSyncList.new(src_path)

list_at_last_sync = lsl.read

delete_remote_deleted_files(src_path, list_at_last_sync - current_list_remote(dest))
rsync_push_new_files(src_path, dest)

lsl.write(current_list_local(src_path))
