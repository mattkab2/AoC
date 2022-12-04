use std::io::BufRead;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let file = std::fs::File::open("../inp/day3.txt")?;
    let buf_read = std::io::BufReader::new(file).lines();
    let inp:Vec<Vec<bool>> = buf_read
    .map(|s| s.unwrap().chars()
    .map(|c| c.to_digit(10).unwrap() != 0)
    .collect())
    .collect();

    let iters: Vec<_> = inp.into_iter()
    .map(|v| v.into_iter())
    .collect();
    let ret: Vec<bool> = iters.into_iter().map(|i| i.next().unwrap())
    .collect::<&[bool]>().count();
    println!("{}",boolToi32(&ret).unwrap());
    Ok(())
}

fn boolToi32(boolInp: &[bool]) -> Result<i32, Box<dyn std::error::Error>> {
    let ret = match boolInp.split_last() {
        Some((true,prefix)) => {
            1 + 2*boolToi32(prefix).unwrap()},
        Some((false,prefix)) => {
            2*boolToi32(prefix).unwrap()},
        None => 0,
    };
    return Ok(ret)
}