#!/usr/bin/env ruby
src_path, dest_host, dest_port, dest_path = ARGV
port ||= 22
require "ostruct"
require 'fileutils'

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

class FileLister
  attr_reader :dir
  def initialize dir
    @dir = File.expand_path(dir)
  end
  def list
    files = _list
    post_process files
  end

  def file_list_cmd
    #ignore dot files/folders
    "cd \"#{dir}\" && find . -type f"
  end

  def post_process list_of_files
    is_hidden = ->(f){f.match(/\/\..*/)}
    list_of_files.split("\n").map{|file| file.gsub(dir, "./").gsub(/^.\//, "")}.reject(&is_hidden)
  end
end

class RemoteFileLister < FileLister
  attr_reader :dest
  def initialize dest
    @dest = dest
    super(dest.path)
  end

  def ssh_command host, port, command
    ret = `ssh #{host} -p #{port} \"#{command}\"`
    if $? != 0
      raise "Command Failed"
    end
    ret
  end

  def _list
    ssh_command dest.host, dest.port, file_list_cmd
  end
end

class LocalFileLister < FileLister
  def initialize src_path
    super(src_path)
  end

  def _list
    `#{file_list_cmd}`
  end
end

def current_list_remote dest
  RemoteFileLister.new(dest).list
end

def delete_remote_deleted_files src_path, list
  #TODO once tested, to add a stage here, before the next stage, to wipe .deleted
  del_folder =File.join(src_path, ".deleted")
  Dir.mkdir del_folder unless File.directory?(del_folder)
  list.each do |file|
    puts "Deleting #{file}"
    #2 stage deletion - to prevent accidental deletion
    #perhaps shift to .deleted folder and delete in 2nd pass

    src    = File.join(src_path, file)
    target = File.join(del_folder, file)
    FileUtils.mkdir_p(File.dirname(target))
    File.rename(src, target)
  end
end

def rsync_push_new_files src, dest
  # ignore existing to optimize for android transfer where i think modification time or something is not being set
  # Files are not expected to be modified on the remote, so this should suffice
  # -W to optimize for large files
  # exclude dot files and folders -http://askubuntu.com/a/482920/92812
  system("rsync -rvzWh --exclude \".*\" --exclude \".*/\" --ignore-existing --progress -e \"ssh -p #{dest.port}\" \"#{src}\" \"#{dest.host}:#{dest.path}\"")
end

def current_list_local path
  LocalFileLister.new(path).list
end

dest = OpenStruct.new(host: dest_host, port: dest_port, path: dest_path)
lsl = LastSyncList.new(src_path)

list_at_last_sync = lsl.read

delete_remote_deleted_files(src_path, list_at_last_sync - current_list_remote(dest))
rsync_push_new_files(src_path, dest)

lsl.write(current_list_local(src_path))
