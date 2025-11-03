#!/usr/bin/env python3
import re
import os
import shutil

def remove_cpp_comments(content):
    """Remove C++ style comments while preserving code"""
    # Remove multi-line comments /* ... */
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
    
    # Remove single-line comments //
    lines = content.split('\n')
    cleaned_lines = []
    for line in lines:
        # Remove // comments but preserve the line structure
        if '//' in line:
            # Find // not in string literals
            parts = line.split('//')
            cleaned_lines.append(parts[0].rstrip())
        else:
            cleaned_lines.append(line)
    
    return '\n'.join(cleaned_lines)

def remove_bash_comments(content):
    """Remove bash comments while preserving shebang and code"""
    lines = content.split('\n')
    cleaned_lines = []
    
    for i, line in enumerate(lines):
        # Keep shebang
        if i == 0 and line.startswith('#!'):
            cleaned_lines.append(line)
            continue
        
        # Remove lines that are pure comments (# at start after whitespace)
        stripped = line.lstrip()
        if stripped.startswith('#') and not line.startswith('#!'):
            continue
        
        # Remove inline comments but keep the code part
        if '#' in line and not line.startswith('#!'):
            # Simple approach: remove # and everything after (might have edge cases)
            parts = line.split('#')
            cleaned_lines.append(parts[0].rstrip())
        else:
            cleaned_lines.append(line)
    
    return '\n'.join(cleaned_lines)

def process_file(src_path, dst_path):
    """Process a single file and remove comments"""
    with open(src_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    if src_path.endswith('.cpp'):
        cleaned_content = remove_cpp_comments(content)
    elif src_path.endswith('.sh'):
        cleaned_content = remove_bash_comments(content)
    else:
        cleaned_content = content
    
    # Remove excessive blank lines (more than 2 consecutive)
    cleaned_content = re.sub(r'\n{3,}', '\n\n', cleaned_content)
    
    os.makedirs(os.path.dirname(dst_path), exist_ok=True)
    with open(dst_path, 'w', encoding='utf-8', newline='\n') as f:
        f.write(cleaned_content)

def main():
    base_dir = r'c:\Users\Lenovo\Downloads\os_prac'
    
    # Process all assignment folders
    for i in range(1, 9):
        src_dir = os.path.join(base_dir, f'ass{i}')
        dst_dir = os.path.join(base_dir, 'no_comments', f'ass{i}')
        
        if not os.path.exists(src_dir):
            continue
        
        print(f"\nProcessing ass{i}...")
        
        for filename in os.listdir(src_dir):
            if filename.endswith(('.cpp', '.sh')):
                src_path = os.path.join(src_dir, filename)
                dst_path = os.path.join(dst_dir, filename)
                
                process_file(src_path, dst_path)
                print(f"  ✓ {filename}")
    
    print("\n✅ All files processed successfully!")

if __name__ == '__main__':
    main()
