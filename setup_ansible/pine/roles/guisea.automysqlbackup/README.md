# Automysqlbackup

[![Build Status](https://travis-ci.org/guisea/ansible-role-automysqlbackup.svg?branch=master)](https://travis-ci.org/guisea/ansible-role-automysqlbackup)

Installs and configures the automysqlbackup utility on RHEL/CentOS or Debian/Ubuntu servers.

## Requirements

No special requirements; note that this role requires root access, so either run it in a playbook with a global `become: yes`, or invoke the role in your playbook like:

    - hosts: mysql-database
      roles:
        - role: guisea.automysqlbackup
          become: yes


---

**Variables**

Available variables are listed below, along with default values (see `defaults/main.yml`):

    automysqlbackup_backup_directory: /var/lib/automysqlbackup
    automysqlbackup_username: "$(grep user ~/.my.cnf | tail -n 1 | cut -d'=' -f2 | awk '{print $1}')"
    automysqlbackup_password: "$(grep password ~/.my.cnf | tail -n 1 | cut -d'=' -f2 | awk '{print $1}')"

By default this role will look for the username and password from ".my.cnf" in the users home directory currently just "root"

## Dependencies

None.

## Example Playbook

    - hosts: db-servers
      become: yes
      vars_files:
        - vars/main.yml
      roles:
        - { role: guisea.automysqlbackup }

*Inside `vars/main.yml`*:

    automysqlbackup_cron:
      minute: 30
      hour: 19
      day: "*"
      month: "*"
      weekday: "*"

## License

MIT / BSD

## Author Information

This role was created in 2017 by [Aaron Guise](https://www.guise.net.nz/)