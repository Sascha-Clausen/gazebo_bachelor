import skidpad

cone_xml_template = '\
    <include>\n\
      <uri>model://{}</uri>\n\
      <name>{}_{}</name>\n\
      <pose>{:.2f} {:.2f} 0 0 0</pose>\n\
    </include>\n\n'

def generate_cone_xml_lines(model_name, cone_positions):
  lines = []
  for i, cone_position in enumerate(cone_positions):
    lines.append(cone_xml_template.format(model_name, model_name, i, cone_position[0], cone_position[1]))
  return lines


skidpad = skidpad.SkidPad()
skidpad.generate()

template_file = open('template.world')
lines = template_file.readlines()

# search for the destination of the robot pose
robot_pose_index = [i for i, s in enumerate(lines) if '<!-- Robot Pose -->' in s][0]
# search for the destination of the cones in the template file
cone_section_index = [i for i, s in enumerate(lines) if '<!-- Cones -->' in s][0]

# write the beginning of the file
skidpad_file = open('skidpad.world', 'w')
skidpad_file.writelines(lines[:robot_pose_index])
# write the robot position
skidpad_file.write('      <pose>{:.2f} {:.2f} 0 0 0</pose>\n'.format(skidpad.starting_pose_front_wing[0],
                                                                     skidpad.starting_pose_front_wing[1]))
# write until the beginning of the cone section
skidpad_file.writelines(lines[robot_pose_index+1:cone_section_index])

#write the cone positions
skidpad_file.writelines(generate_cone_xml_lines('yellow_cone', skidpad.cones_left))
skidpad_file.writelines(generate_cone_xml_lines('blue_cone', skidpad.cones_right))
skidpad_file.writelines(generate_cone_xml_lines('large_orange_cone', skidpad.orange_big_cones))
skidpad_file.writelines(generate_cone_xml_lines('orange_cone', skidpad.orange_cones))

# write rest of template file
skidpad_file.writelines(lines[cone_section_index+1:])

skidpad_file.close()

